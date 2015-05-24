/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// cvar.c -- dynamic variable tracking

#ifdef SERVERONLY 
#include "qwsvdef.h"
#else
#include "quakedef.h"
#endif
#include "dstring.h" // FS: dstring

cvar_t	*cvar_vars;
char	*cvar_null_string = "";
void Cvar_ParseDeveloperFlags (void); // FS: Special stuff for showing all the dev flags

// FS: Cvar_List_f from Quakespasm
void Cvar_List_f (void)
{
	cvar_t	*cvar;
        char      *partial;
	int		len, count;

	if (Cmd_Argc() > 1)
	{
		partial = Cmd_Argv (1);
		len = Q_strlen(partial);
	}
	else
	{
		partial = NULL;
		len = 0;
	}

	count=0;
	for (cvar=cvar_vars ; cvar ; cvar=cvar->next)
	{
		if (partial && Q_strncmp (partial,cvar->name, len))
		{
			continue;
		}
		Con_SafePrintf ("%s%s %s \"%s\"\n",
			cvar->archive ? "*" : " ",
			cvar->info ? "s" : " ", // FS: Not server here...
			cvar->name,
			cvar->string);
		count++;
	}

	Con_SafePrintf ("%i cvars", count);
	if (partial)
	{
		Con_SafePrintf (" beginning with \"%s\"", partial);
	}
	Con_SafePrintf ("\n");
}


/*
============
Cvar_FindVar
============
*/
cvar_t *Cvar_FindVar (const char *var_name)
{
	cvar_t	*var;
	
	for (var=cvar_vars ; var ; var=var->next)
		if (!Q_strcmp ((char *)var_name, var->name))
			return var;

	return NULL;
}

/*
============
Cvar_VariableValue
============
*/
float	Cvar_VariableValue (char *var_name)
{
	cvar_t	*var;
	
	var = Cvar_FindVar (var_name);
	if (!var)
		return 0;
	return Q_atof (var->string);
}


/*
============
Cvar_VariableString
============
*/
char *Cvar_VariableString (char *var_name)
{
	cvar_t *var;
	
	var = Cvar_FindVar (var_name);
	if (!var)
		return cvar_null_string;
	return var->string;
}


/*
============
Cvar_CompleteVariable
============
*/
char *Cvar_CompleteVariable (char *partial)
{
	cvar_t		*cvar;
	int			len;
	
	len = Q_strlen(partial);
	
	if (!len)
		return NULL;
		
	// check exact match
	for (cvar=cvar_vars ; cvar ; cvar=cvar->next)
		if (!strcmp (partial,cvar->name))
			return cvar->name;

	// check partial match
	for (cvar=cvar_vars ; cvar ; cvar=cvar->next)
		if (!Q_strncmp (partial,cvar->name, len))
			return cvar->name;

	return NULL;
}


#ifdef SERVERONLY
void SV_SendServerInfoChange(char *key, char *value);
#endif

/*
============
Cvar_Set
============
*/
void Cvar_Set (char *var_name, char *value)
{
	cvar_t	*var;
	
	var = Cvar_FindVar (var_name);
	if (!var)
	{	// there is an error in C code if this happens
		Con_Printf ("Cvar_Set: variable %s not found\n", var_name);
		return;
	}

#ifdef SERVERONLY
	if (var->info)
	{
		Info_SetValueForKey (svs.info, var_name, value, MAX_SERVERINFO_STRING);
		SV_SendServerInfoChange(var_name, value);
//		SV_BroadcastCommand ("fullserverinfo \"%s\"\n", svs.info);
	}
#else
	if (var->info)
	{
		Info_SetValueForKey (cls.userinfo, var_name, value, MAX_INFO_STRING);
		if (cls.state >= ca_connected)
		{
			MSG_WriteByte (&cls.netchan.message, clc_stringcmd);
			SZ_Print (&cls.netchan.message, va("setinfo \"%s\" \"%s\"\n", var_name, value));
		}
	}
#endif
	
	Z_Free (var->string);	// free the old value string
	
	var->string = Z_Malloc (Q_strlen(value)+1);
	Q_strcpy (var->string, value);
	var->value = Q_atof (var->string);
	var->intValue = Q_atoi (var->string); // FS: Added
}

/*
============
Cvar_SetValue
============
*/
void Cvar_SetValue (char *var_name, float value)
{
/*
	char	val[32];
	
	sprintf (val, "%f",value);
*/ // FS: New school dstring
	dstring_t *val;
	val = dstring_new ();
	if (value == (int)value) // FS: Weird zeros fix from QIP
		dsprintf(val, "%d", (int)value);
	else
		dsprintf(val, "%f",value); 
	Cvar_Set (var_name, val->str);
	dstring_delete (val);
}


/*
============
Cvar_RegisterVariable

Adds a freestanding variable to the variable list.
============
*/
void Cvar_RegisterVariable (cvar_t *variable)
{
	char    value[512];
// first check to see if it has allready been defined
	if (Cvar_FindVar (variable->name))
	{
		Con_Printf ("Can't register variable %s, allready defined\n", variable->name);
		return;
	}
	
// check for overlap with a command
	if (Cmd_Exists (variable->name))
	{
		Con_Printf ("Cvar_RegisterVariable: %s is a command\n", variable->name);
		return;
	}
		
// link the variable in
	variable->next = cvar_vars;
	cvar_vars = variable;

// copy the value off, because future sets will Z_Free it
	strcpy (value, variable->string);
	variable->string = Z_Malloc (1);	
	variable->defaultString = Z_Malloc (Q_strlen(variable->string)+1); // FS
	Q_strcpy((char *)variable->defaultString, value); // FS
	
// set it through the function to be consistant
	Cvar_Set (variable->name, value);
}

/*
============
Cvar_Command

Handles variable inspection and changing from the console
============
*/
qboolean	Cvar_Command (void)
{
	cvar_t			*v;

// check variables
	v = Cvar_FindVar (Cmd_Argv(0));
	if (!v)
		return false;

	if (!Q_strcmp(v->name, "developer") && con_show_dev_flags.intValue) // FS: Special case for showing enabled flags
	{
		if(Q_strlen(Cmd_Argv(1)) > 0)
			Cvar_Set(developer.name, Cmd_Argv(1));
		Cvar_ParseDeveloperFlags();
		return true;
	}

// perform a variable print or set
	if (Cmd_Argc() == 1)
	{
		Con_Printf ("\"%s\" is \"%s\".  Default: \"%s\".\n", v->name, v->string, v->defaultString);
		if (con_show_description.value && v->description != NULL && v->description[0] != 0)
			Con_Printf("Description: %s\n", v->description);
		return true;
	}

	Cvar_Set (v->name, Cmd_Argv(1));
	return true;
}


/*
============
Cvar_WriteVariables

Writes lines containing "set variable value" for all variables
with the archive flag set to true.
============
*/
void Cvar_WriteVariables (FILE *f)
{
	cvar_t	*var;
	
	for (var = cvar_vars ; var ; var = var->next)
		if (var->archive)
			fprintf (f, "%s \"%s\"\n", var->name, var->string);
}

void Cvar_Init (void) // FS: johnfitz
{
	Cmd_AddCommand ("cvarlist", Cvar_List_f);
}

void Cvar_Set_Description (const char *var_name, const char *description) // FS
{
	cvar_t	*var;
	var = Cvar_FindVar (var_name);
	if (!var)
	{
		Con_DPrintf(DEVELOPER_MSG_STANDARD, "Error: Can't set description for %s!\n", var_name);
		return;
	}
	var->description = description;
}

void Cvar_ParseDeveloperFlags (void) // FS: Special stuff for showing all the dev flags
{
	Con_Printf("\"%s\" is \"%s\", Default: \"%s\"\n", developer.name, developer.string, developer.defaultString);
	if(developer.intValue > 0)
	{
		unsigned long devFlags = 0;
		if(developer.intValue == 1)
			devFlags = 65534;
		else
			devFlags = (unsigned long)developer.value;
		Con_Printf("Toggled flags:\n");
		if(devFlags & DEVELOPER_MSG_STANDARD)
			Con_Printf(" * Standard messages - 2\n");
		if(devFlags & DEVELOPER_MSG_SOUND)
			Con_Printf(" * Sound messages - 4\n");
		if(devFlags & DEVELOPER_MSG_NET)
			Con_Printf(" * Network messages - 8\n");
		if(devFlags & DEVELOPER_MSG_IO)
			Con_Printf(" * File IO messages - 16\n");
		if(devFlags & DEVELOPER_MSG_VIDEO)
			Con_Printf(" * Graphics Renderer messages - 32\n");
		if(devFlags & DEVELOPER_MSG_CD)
			Con_Printf(" * CD Player messages - 64\n");
		if(devFlags & DEVELOPER_MSG_MEM)
			Con_Printf(" * Memory messages - 128\n");
//		if(devFlags & DEVELOPER_MSG_SERVER)
//			Con_Printf(" * Server messages - 256\n");
//		if(devFlags & DEVELOPER_MSG_PROGS)
//			Con_Printf(" * Prog messages - 512\n");
//		if(devFlags & DEVELOPER_MSG_WORLD)
//			Con_Printf(" * World.dll messages - 1024\n");
		if(devFlags & DEVELOPER_MSG_PHYSICS)
			Con_Printf(" * Physics messages - 2048\n");
//		if(devFlags & DEVELOPER_MSG_WEAPONS)
//			Con_Printf(" * Weapons.dll messages - 4096\n");
//		if(devFlags & DEVELOPER_MSG_GCE)
//			Con_Printf(" * GCE.dll messages - 8192\n");
		if(devFlags & DEVELOPER_MSG_ENTITY)
			Con_Printf(" * Entity messages - 16384\n");
//		if(devFlags & DEVELOPER_MSG_SAVE)
//			Con_Printf(" * Save/Restore messages - 32768\n");
		if(devFlags & DEVELOPER_MSG_VERBOSE)
			Con_Printf(" * Extremely Verbose messages - 65536\n");
		if(devFlags & DEVELOPER_MSG_GAMESPY)
			Con_Printf(" * Extremely Verbose GameSpy messages - 131072\n");
	}
	else
	{
		if (developer.description && con_show_description.intValue) // FS
			Con_Printf("Description: %s\n", developer.description);
	}
}
