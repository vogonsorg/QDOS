#ifdef USE_CURL
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

*/
#ifdef _WIN32
#define CURL_HIDDEN_SYMBOLS
#define CURL_EXTERN_SYMBOL
#define CURL_CALLING_CONVENTION __cdecl
#endif

#define CURL_STATICLIB
#include <curl/curl.h>

#include "sys.h"

#include "cl_http.h"
#include "quakedef.h"

static int curl_borked;
static CURL *easy_handle;
static CURLM *multi_handle;
static qboolean httpDlAborted = false;

/* FS: For KBps calculator */
int		prevSize;

void CL_HTTP_Reset_KBps_Counter (void)
{
	prevSize = 0;
	CL_Download_Reset_KBps_counter ();
}

void CL_HTTP_Calculate_KBps (int curSize, int totalSize)
{
	int byteDistance = curSize-prevSize;

	CL_Download_Calculate_KBps (byteDistance, totalSize);
	prevSize = curSize;
}

void CL_HTTP_RemoveTemp(void)
{
	char removeFile[MAX_OSPATH];

	if (cls.download)
		fclose(cls.download);

	Com_sprintf(removeFile, sizeof(removeFile), "%s/%s", com_gamedir, cls.downloadtempname->str);
	Con_DPrintf(DEVELOPER_MSG_NET, "Removing temp file: %s\n", cls.downloadtempname->str);
	if(remove(removeFile) != 0)
		Con_Printf("Error removing file: %s\n", cls.downloadtempname->str);
}

static int http_progress (void *clientp, double dltotal, double dlnow,
			   double ultotal, double uplow)
{
	if (dltotal)
	{
		CL_HTTP_Calculate_KBps((int)dlnow, (int)dltotal);
		cls.downloadpercent = (int)((dlnow / dltotal) * 100.0f);
	}
	else
		cls.downloadpercent = 0;
	return 0;	//non-zero = abort
}


static size_t http_write (void *ptr, size_t size, size_t nmemb, void *stream)
{
	if (!cls.download)
	{
		/* FS: If this fails here delete the temp file, don't make it go to response code 200! */
		httpDlAborted = true;
		Con_DPrintf (DEVELOPER_MSG_NET, "http_write: unexpected call, likely transfer manually aborted\n");
		return -1;
	}
	return fwrite (ptr, 1, size *nmemb, cls.download);
}

void CL_HTTP_Init (void)
{
	if ((curl_borked = curl_global_init (CURL_GLOBAL_NOTHING)))
		return;
	multi_handle = curl_multi_init ();
}

void CL_HTTP_Shutdown (void)
{
	if (curl_borked)
		return;
	curl_multi_cleanup (multi_handle);
	curl_global_cleanup ();
}

void CL_HTTP_StartDownload (void)
{
	Con_DPrintf(DEVELOPER_MSG_NET, "In CL_HTTP_StartDownload\n");
	CL_HTTP_Reset_KBps_Counter();

	easy_handle = curl_easy_init ();

	curl_easy_setopt (easy_handle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt (easy_handle, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt (easy_handle, CURLOPT_PROGRESSFUNCTION, http_progress);
	curl_easy_setopt (easy_handle, CURLOPT_WRITEFUNCTION, http_write);
	curl_easy_setopt (easy_handle, CURLOPT_URL, cls.downloadurl->str);
	curl_multi_add_handle (multi_handle, easy_handle);

	Con_DPrintf(DEVELOPER_MSG_NET, "HTTP Download URL: %s\n", cls.downloadurl->str);
}

void CL_HTTP_Update (void)
{
	int         running_handles;
	int         messages_in_queue;
	CURLMsg    *msg;

	curl_multi_perform (multi_handle, &running_handles);
	while ((msg = curl_multi_info_read (multi_handle, &messages_in_queue)))
	{
		if (msg->msg == CURLMSG_DONE)
		{
			long        response_code;

			curl_easy_getinfo (msg->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
			Con_DPrintf(DEVELOPER_MSG_NET, "HTTP URL response code: %li\n", response_code);
			if ( (response_code == HTTP_OK || response_code == HTTP_REST) && !(httpDlAborted)) /* FS: Have to check for the abort boolean, very rarely a ctrl+c to stop the transfer ends up here instead and thinks it's a good file to load fucking everything up */
			{
				Con_Printf ("HTTP Download of %s completed\n", cls.downloadname->str); /* FS: Tell me when it's done */

				CL_FinishDownload (true); /* FS: Temp file renames are handled here if successful */
			}
			else
			{
				Con_Printf ("HTTP download failed: %ld\n", response_code);

				CL_HTTP_RemoveTemp(); /* FS: Remove stray temp files so the server doesn't think we're resuming 0 length files or files that now have just header information */

				if(httpDlAborted)
					httpDlAborted = false;

				CL_FinishDownload (false);
			}
			CL_HTTP_Reset();
		}
	}
}

void CL_HTTP_Reset (void)
{
	curl_multi_remove_handle (multi_handle, easy_handle);
	curl_easy_cleanup (easy_handle);
	easy_handle = 0;
}

#else

#include "cl_http.h"

void CL_HTTP_Init (void) {}
void CL_HTTP_Shutdown (void) {}
void CL_HTTP_StartDownload (void) {}
void CL_HTTP_Update (void) {}
void CL_HTTP_Reset (void) {}

#endif 
