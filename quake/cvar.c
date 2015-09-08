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

#include "quakedef.h"

cvar_t	*cvar_vars;
void Cvar_ParseDeveloperFlags (void); /* FS: Special stuff for showing all the dev flags */

/*
============
Cvar_InfoValidate
============
*/
static qboolean Cvar_InfoValidate (char *s)
{
	if (strstr (s, "\\"))
		return false;
	if (strstr (s, "\""))
		return false;
	if (strstr (s, ";"))
		return false;
	return true;
}

/* FS: Cvar_List_f from Quakespasm */
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
			cvar->flags & CVAR_ARCHIVE ? "*" : " ",
			cvar->flags & CVAR_SERVERINFO ? "s" : " ",
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
		return "";
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
		
// check functions
	for (cvar=cvar_vars ; cvar ; cvar=cvar->next)
		if (!Q_strncmp (partial,cvar->name, len))
			return cvar->name;

	return NULL;
}


/*
============
Cvar_Get

If the variable already exists, the value will not be set
The flags will be or'ed in if the variable exists.
============
*/
cvar_t *Cvar_Get (char *var_name, char *var_value, int flags)
{
	cvar_t	*var;

	if (flags & CVAR_SERVERINFO)
	{
		if (!Cvar_InfoValidate (var_name))
		{
			Con_Printf("invalid info cvar name\n");
			return NULL;
		}
	}

	var = Cvar_FindVar (var_name);
	if (var)
	{
		var->flags |= flags;
		// Knightmare- change default value if this is called again
		Z_Free(var->defaultString);
		var->defaultString = CopyString(var_value);
		var->defaultFlags |= flags; /* FS: Ditto */

		return var;
	}

	if (!var_value)
		return NULL;

	if (flags & CVAR_SERVERINFO)
	{
		if (!Cvar_InfoValidate (var_value))
		{
			Con_Printf("invalid info cvar value\n");
			return NULL;
		}
	}

	var = Z_Malloc (sizeof(*var));
	var->name = CopyString (var_name);
	var->string = CopyString (var_value);
	var->modified = true;
	var->value = atof (var->string);
	var->intValue = atoi(var->string); /* FS: So we don't need to cast shit all the time */
	var->defaultString = CopyString(var_value); /* FS: Find out what it was initially */
	var->defaultFlags = flags; /* FS: Default flags for resetcvar */
	var->description = NULL; /* FS: Init it first, d'oh */

	// link the variable in
	var->next = cvar_vars;
	cvar_vars = var;

	var->flags = flags;

	return var;
}

/*
============
Cvar_Set2
============
*/
cvar_t *Cvar_Set2 (char *var_name, char *value, qboolean force)
{
	cvar_t	*var;
	
	var = Cvar_FindVar (var_name);
	if (!var)
	{	// create it
		return Cvar_Get (var_name, value, 0);
	}

	if (var->flags & CVAR_SERVERINFO)
	{
		if (!Cvar_InfoValidate (value))
		{
			Con_Printf("invalid info cvar value\n");
			return var;
		}
	}

	if (!force)
	{
		if (var->flags & CVAR_NOSET)
		{
			Con_Printf ("%s is write protected.\n", var_name);
			return var;
		}

		if (var->flags & CVAR_LATCH)
		{
			if (var->latched_string)
			{
				if (strcmp(value, var->latched_string) == 0)
					return var;
				Z_Free (var->latched_string);
			}
			else
			{
				if (strcmp(value, var->string) == 0)
					return var;
			}

			if (sv.active)
			{
				Con_Printf ("%s will be changed for next map.\n", var_name);
				var->latched_string = CopyString(value);
			}
			else
			{
				var->string = CopyString(value);
				var->value = atof (var->string);
				var->intValue = atoi(var->string); /* FS: So we don't need to cast shit all the time */
			}
			return var;
		}
	}
	else
	{
		if (var->latched_string)
		{
			Z_Free (var->latched_string);
			var->latched_string = NULL;
		}
	}

	if (!strcmp(value, var->string))
		return var;		// not changed

	var->modified = true; /* FS: Added */

	Z_Free (var->string);	// free the old value string

	var->string = CopyString(value);
	var->value = atof (var->string);
	var->intValue = atoi(var->string); /* FS: So we don't need to cast shit all the time */

	if (var->flags & CVAR_SERVERINFO)
	{
		if (sv.active)
			SV_BroadcastPrintf ("\"%s\" changed to \"%s\"\n", var->name, var->string);
	}

	return var;
}

/*
============
Cvar_ForceSet
============
*/
cvar_t *Cvar_ForceSet (char *var_name, char *value)
{
	return Cvar_Set2 (var_name, value, true);
}

/*
============
Cvar_Set
============
*/
cvar_t *Cvar_Set (char *var_name, char *value)
{
	return Cvar_Set2 (var_name, value, false);
}

/*
============
Cvar_FullSet
============
*/
cvar_t *Cvar_FullSet (char *var_name, char *value, int flags)
{
	cvar_t	*var;

	var = Cvar_FindVar (var_name);
	if (!var)
	{	// create it
		return Cvar_Get (var_name, value, flags);
	}

	var->modified = true;

	Z_Free (var->string);	// free the old value string

	var->string = CopyString(value);
	var->value = atof (var->string);
	var->intValue = atoi(var->string); /* FS: So we don't need to cast shit all the time */
	var->flags = flags;

	return var;
}

/*
============
Cvar_SetValue
============
*/
void Cvar_SetValue (char *var_name, float value)
{
	dstring_t *val;

	val = dstring_new ();

	if (value == (int)value) /* FS: Weird zeros fix from QIP */
		dsprintf(val, "%d", (int)value);
	else
		dsprintf(val, "%f",value); 

	Cvar_Set (var_name, val->str);
	dstring_delete (val);
}

/*
============
Cvar_GetLatchedVars

Any variables with latched values will now be updated
============
*/
void Cvar_GetLatchedVars (void)
{
	cvar_t	*var;

	for (var = cvar_vars ; var ; var = var->next)
	{
		if (!var->latched_string)
			continue;
		Z_Free (var->string);
		var->string = var->latched_string;
		var->latched_string = NULL;
		var->value = atof(var->string);
		var->intValue = atoi(var->string); /* FS: So we don't need to cast shit all the time */
	}
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

	if (!Q_strcmp(v->name, "developer") && con_show_dev_flags->intValue) /* FS: Special case for showing enabled flags */
	{
		if(Q_strlen(Cmd_Argv(1)) > 0)
			Cvar_Set(developer->name, Cmd_Argv(1));
		Cvar_ParseDeveloperFlags();
		return true;
	}

// perform a variable print or set
	if (Cmd_Argc() == 1)
	{
		if ( (v->flags & CVAR_LATCH) && v->latched_string)
			Con_Printf ("\"%s\" is \"%s\", Default: \"%s\", Latched to: \"%s\"\n", v->name, v->string, v->defaultString, v->latched_string);
		else
			Con_Printf ("\"%s\" is \"%s\",  Default: \"%s\".\n", v->name, v->string, v->defaultString);

		/* FS: cvar descriptions */
		/* FS: Always show it for con_show_description so we know what it does */
		if (v->description) {
		    if (con_show_description->intValue || v == con_show_description)
			Con_Printf("Description: %s\n", v->description);
		}

		return true;
	}

	Cvar_Set (v->name, Cmd_Argv(1));
	return true;
}


/*
============
Cvar_Set_f

Allows setting and defining of arbitrary cvars from console
============
*/
void Cvar_Set_f (void)
{
	int		c;
	int		flags;

	c = Cmd_Argc();
	if (c != 3 && c != 4)
	{
		Con_Printf ("usage: set <variable> <value> [s]\n");
		return;
	}

	if (c == 4)
	{
		if (!strcmp(Cmd_Argv(3), "s"))
			flags = CVAR_SERVERINFO;
		else
		{
			Con_Printf ("flags can only be 's'\n");
			return;
		}
		Cvar_FullSet (Cmd_Argv(1), Cmd_Argv(2), flags);
	}
	else
		Cvar_Set (Cmd_Argv(1), Cmd_Argv(2));
}


/*
============
Cvar_WriteVariables

Writes lines containing "set variable value" for all variables
with the archive flag set to true.
============
*/
void Cvar_WriteVariables (const char *path)
{
	cvar_t	*var;
	char	buffer[1024];
	FILE	*f;
	
	f = fopen (path, "a");
	for (var = cvar_vars ; var ; var = var->next)
	{
		if (var->flags & CVAR_ARCHIVE)
		{
			Com_sprintf (buffer, sizeof(buffer), "set %s \"%s\"\n", var->name, var->string);
			fprintf (f, "%s", buffer);
		}
	}
	fclose (f);
}

void Cvar_Init (void) /* FS: from fitzquake */
{
	Cmd_AddCommand ("set", Cvar_Set_f);
	Cmd_AddCommand ("cvarlist", Cvar_List_f);
}

void Cvar_Set_Description (const char *var_name, const char *description) /* FS: Added */
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

void Cvar_ParseDeveloperFlags (void) /* FS: Special stuff for showing all the dev flags */
{
	Con_Printf("\"%s\" is \"%s\", Default: \"%s\"\n", developer->name, developer->string, developer->defaultString);
	if(developer->intValue > 0)
	{
		unsigned long devFlags = 0;
		if(developer->intValue == 1)
			devFlags = 65534;
		else
			devFlags = (unsigned long)developer->value;
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
		if(devFlags & DEVELOPER_MSG_SERVER)
			Con_Printf(" * Server messages - 256\n");
		if(devFlags & DEVELOPER_MSG_PROGS)
			Con_Printf(" * Prog messages - 512\n");
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
		if(devFlags & DEVELOPER_MSG_SAVE)
			Con_Printf(" * Save/Restore messages - 32768\n");
		if(devFlags & DEVELOPER_MSG_VERBOSE)
			Con_Printf(" * Extremely Verbose messages - 65536\n");
	}
	else
	{
		if (developer->description && con_show_description->intValue)
			Con_Printf("Description: %s\n", developer->description);
	}
}
