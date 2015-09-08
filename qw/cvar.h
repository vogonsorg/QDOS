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

#ifndef __CVAR_H
#define __CVAR_H

#define	CVAR_ARCHIVE	1	// set to cause it to be saved to vars.rc
#define	CVAR_USERINFO	2	// added to userinfo  when changed /* FS: QW not Q1 */
#define	CVAR_SERVERINFO	4	// added to serverinfo when changed
#define	CVAR_NOSET		8	// don't allow change from console at all,
							// but can be set from the command line
#define	CVAR_LATCH		16	// save changes until server restart

typedef struct cvar_s
{
	char	*name;
	char	*string;
	char		*latched_string; /* FS: Added */
	int			flags;
	qboolean	modified; /* FS: Added */
	float	value;
	int		intValue; /* FS: Added */
	char		*defaultString; /* FS: Added */
	const char	*description; /* FS: Added */
	int			defaultFlags; /* FS: Added */
	struct cvar_s *next;
} cvar_t;

extern cvar_t	*cvar_vars;

cvar_t	*Cvar_Get (char *var_name, char *value, int flags);
// creates the variable if it doesn't exist, or returns the existing one
// if it exists, the value will not be changed, but flags will be ORed in
// that allows variables to be unarchived without needing bitflags

cvar_t	*Cvar_Set (char *var_name, char *value);
// will create the variable if it doesn't exist

cvar_t	*Cvar_ForceSet (char *var_name, char *value);
// will set the variable even if NOSET or LATCH

cvar_t	*Cvar_FullSet (char *var_name, char *value, int flags);

void	Cvar_SetValue (char *var_name, float value);
// expands value to a string and calls Cvar_Set

float	Cvar_VariableValue (char *var_name);
// returns 0 if not defined or non numeric

char	*Cvar_VariableString (char *var_name);
// returns an empty string if not defined

char 	*Cvar_CompleteVariable (char *partial);
// attempts to match a partial variable name for command line completion
// returns NULL if nothing fits

void	Cvar_GetLatchedVars (void);
// any CVAR_LATCHED variables that have been set will now take effect

qboolean Cvar_Command (void);
// called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
// command.  Returns true if the command was a variable reference that
// was handled. (print or change)

void 	Cvar_WriteVariables (const char *path);
// Writes lines containing "set variable value" for all variables
// with the archive flag set to true.

void	Cvar_Init (void);

cvar_t	*Cvar_FindVar (const char *var_name); /* FS: Added */

void	Cvar_Set_Description (const char *var_name, const char *description); /* FS: Added */

#endif // __CVAR_H
