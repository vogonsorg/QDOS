/*
	#FILENAME#

	#DESCRIPTION#

	Copyright (C) 2007 #AUTHOR#

	Author: #AUTHOR#
	Date: #DATE#

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

	$Id: template.h 11394 2007-03-17 03:23:39Z taniwha $
*/

#ifndef __cl_http_h
#define __cl_http_h

#define HTTP_OK 200
#define HTTP_REST 206
#define HTTP_UNAUTHORIZED 401
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404

void CL_HTTP_Init (void);
void CL_HTTP_Shutdown (void);
void CL_HTTP_StartDownload (void);
void CL_HTTP_Update (void);
void CL_HTTP_Reset (void);

#endif//__cl_http_h
