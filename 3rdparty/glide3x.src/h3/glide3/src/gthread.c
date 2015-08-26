/*
** THIS SOFTWARE IS SUBJECT TO COPYRIGHT PROTECTION AND IS OFFERED ONLY
** PURSUANT TO THE 3DFX GLIDE GENERAL PUBLIC LICENSE. THERE IS NO RIGHT
** TO USE THE GLIDE TRADEMARK WITHOUT PRIOR WRITTEN PERMISSION OF 3DFX
** INTERACTIVE, INC. A COPY OF THIS LICENSE MAY BE OBTAINED FROM THE 
** DISTRIBUTOR OR BY CONTACTING 3DFX INTERACTIVE INC(info@3dfx.com). 
** THIS PROGRAM IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER 
** EXPRESSED OR IMPLIED. SEE THE 3DFX GLIDE GENERAL PUBLIC LICENSE FOR A
** FULL TEXT OF THE NON-WARRANTY PROVISIONS.  
** 
** USE, DUPLICATION OR DISCLOSURE BY THE GOVERNMENT IS SUBJECT TO
** RESTRICTIONS AS SET FORTH IN SUBDIVISION (C)(1)(II) OF THE RIGHTS IN
** TECHNICAL DATA AND COMPUTER SOFTWARE CLAUSE AT DFARS 252.227-7013,
** AND/OR IN SIMILAR OR SUCCESSOR CLAUSES IN THE FAR, DOD OR NASA FAR
** SUPPLEMENT. UNPUBLISHED RIGHTS RESERVED UNDER THE COPYRIGHT LAWS OF
** THE UNITED STATES.  
** 
** COPYRIGHT 3DFX INTERACTIVE, INC. 1999, ALL RIGHTS RESERVED
**
**
** $Revision: 1.2.6.4 $ 
** $Date: 2005/05/25 08:56:24 $ 
**
*/


/* NOTE: This file is compiled to naught if we aren't
   running under Win32 */

#if defined( __WIN32__ )

#include <3dfx.h>
#include <glidesys.h>

#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>

#include "fxglide.h"
#include "fxcmd.h"

#if (GLIDE_PLATFORM & GLIDE_OS_WIN32)
#include <windows.h>
#endif

static CRITICAL_SECTION criticalSectionObject;
static DWORD            tlsIndex;

static FxBool threadInit;
static FxBool criticalSectionInit;

void 
initThreadStorage( void ) 
{
  if ( !threadInit ) {
    threadInit = 1;
    _GlideRoot.tlsIndex = TlsAlloc();
  }
  if (_GlideRoot.OSWin95)
    _GlideRoot.tlsOffset = W95_TLS_INDEX_TO_OFFSET(_GlideRoot.tlsIndex);
  else
    _GlideRoot.tlsOffset = WNT_TLS_INDEX_TO_OFFSET(_GlideRoot.tlsIndex);

} /* initThreadStorage */

void setThreadValue( unsigned long value ) {
    GR_CHECK_F( "setThreadValue", !threadInit, "Thread storage not initialized\n" );
    TlsSetValue( _GlideRoot.tlsIndex, (void*)value );
}

#pragma warning (4:4035)        /* No return value */
unsigned long getThreadValueSLOW( void ) {
    GR_CHECK_F( "getThreadValue", !threadInit, "Thread storage not initialized\n" );

#if 0
    return (FxU32)TlsGetValue( _GlideRoot.tlsIndex );
#elif 1
    __GR_GET_TLSC_VALUE();
#else
    __asm {
      __asm mov esi, DWORD PTR fs:[WNT_TEB_PTR] 
      __asm add esi, DWORD PTR _GlideRoot.tlsOffset \
     __asm mov eax, DWORD PTR [esi] \
}

#endif

}

void initCriticalSection( void ) {
    if ( !criticalSectionInit ) {
        criticalSectionInit = 1;
        InitializeCriticalSection( &criticalSectionObject );
    }
}

void beginCriticalSection( void ) {
    GR_CHECK_F( "beginCriticalSection", !criticalSectionInit, "Critical section not initialized\n" );
    EnterCriticalSection( &criticalSectionObject );
}

void endCriticalSection( void ) {
    GR_CHECK_F( "endCriticalSection", !criticalSectionInit, "Critical section not initialized\n" );
    LeaveCriticalSection( &criticalSectionObject );
}

#elif defined(macintosh)

#include <3dfx.h>
#include <glidesys.h>

#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>

#include "fxglide.h"
#include "fxcmd.h"

FxU32 _threadValueMacOS;

void initThreadStorage(void)
{
}

void setThreadValue( unsigned long value )
{
	_threadValueMacOS = value;
}

FxU32 getThreadValueSLOW( void )
{
	return _threadValueMacOS;
}
 
void initCriticalSection(void)
{
}

void beginCriticalSection(void)
{
}

void endCriticalSection(void)
{
}

#elif defined(__linux__) || defined(__FreeBSD__)


#include <3dfx.h>
#include <glidesys.h>

#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>

#include "fxglide.h"
#include "fxcmd.h"

unsigned long threadValueLinux;

void initThreadStorage(void)
{
}

void setThreadValue( unsigned long value )
{
	threadValueLinux = value;
}

unsigned long getThreadValueSLOW( void )
{
	return threadValueLinux;
}
 
void initCriticalSection(void)
{
}

void beginCriticalSection(void)
{
}

void endCriticalSection(void)
{
}

#elif (GLIDE_PLATFORM & GLIDE_OS_DOS32)


#include <3dfx.h>
#include <glidesys.h>

#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>

#include "fxglide.h"
#include "fxcmd.h"

FxU32 GR_CDECL threadValueDJGPP;

void initThreadStorage(void)
{
}

void setThreadValue( FxU32 value )
{
	threadValueDJGPP = value;
}

FxU32 getThreadValueSLOW( void )
{
	return threadValueDJGPP;
}
 
void initCriticalSection(void)
{
}

void beginCriticalSection(void)
{
}

void endCriticalSection(void)
{
}

#else
#  error "No thread synchronization/storage functions defined for this OS"
#endif
