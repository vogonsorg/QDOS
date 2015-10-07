# Microsoft Developer Studio Project File - Name="qwcl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=qwcl - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "qwcl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "qwcl.mak" CFG="qwcl - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "qwcl - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "qwcl - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "qwcl - Win32 GL QW" (based on "Win32 (x86) Application")
!MESSAGE "qwcl - Win32 GL QW Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MT /GX /O2 /I "..\common\win32" /I "..\common\win32\dxsdk\sdk\inc" /I "..\common\win32\scitech\include" /I "winquake" /I "..\common" /I ".\\" /D "QUAKEWORLD" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OGG_SUPPORT" /D "GAMESPY" /D "USE_CURL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\common\win32\scitech\lib\win32\vc\mgllt.lib ..\common\win32\dxsdk\sdk\lib\dxguid.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /profile /machine:I386 /out:".\Release/WQWDOS.EXE"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /I "..\common\win32" /I "..\common\win32\dxsdk\sdk\inc" /I "..\common\win32\scitech\include" /I "winquake" /I "..\common" /I ".\\" /D "QUAKEWORLD" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OGG_SUPPORT" /D "GAMESPY" /D "USE_CURL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ..\common\win32\scitech\lib\win32\vc\mgllt.lib ..\common\win32\dxsdk\sdk\lib\dxguid.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:".\Debug/DWQWDOS.EXE"
# SUBTRACT LINK32 /profile /nodefaultlib

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\release_gl"
# PROP BASE Intermediate_Dir ".\release_gl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\release_gl"
# PROP Intermediate_Dir ".\release_gl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MT /GX /O2 /I "..\common\win32" /I "..\common\win32\dxsdk\sdk\inc" /I "..\common\win32\scitech\include" /I "winquake" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OGG_SUPPORT" /D "GAMESPY" /D "USE_CURL" /FR /YX /FD /c
# ADD CPP /nologo /G5 /MT /GX /O2 /I "..\common\win32" /I "..\common\win32\dxsdk\sdk\inc" /I "..\common\win32\scitech\include" /I "winquake" /I "..\common" /I ".\\" /D "QUAKEWORLD" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OGG_SUPPORT" /D "GAMESPY" /D "USE_CURL" /D "GLQUAKE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\common\win32\scitech\lib\win32\vc\mgllt.lib ..\common\win32\dxsdk\sdk\lib\dxguid.lib opengl32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /profile /machine:I386 /out:".\Release/WQWDOS.EXE"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 ..\common\win32\dxsdk\sdk\lib\dxguid.lib comctl32.lib winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /machine:I386 /nodefaultlib:"libcmtd.lib" /out:".\release_gl/GLQW2.EXE"
# SUBTRACT LINK32 /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\debug_gl"
# PROP BASE Intermediate_Dir ".\debug_gl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\debug_gl"
# PROP Intermediate_Dir ".\debug_gl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MT /GX /O2 /I "..\common\win32" /I "..\common\win32\dxsdk\sdk\inc" /I "..\common\win32\scitech\include" /I "winquake" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OGG_SUPPORT" /D "GAMESPY" /D "USE_CURL" /D "GLQUAKE" /FR /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /I "..\common\win32" /I "..\common\win32\dxsdk\sdk\inc" /I "..\common\win32\scitech\include" /I "winquake" /I "..\common" /I ".\\" /D "QUAKEWORLD" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OGG_SUPPORT" /D "GAMESPY" /D "USE_CURL" /D "GLQUAKE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /fo".\debug_gl/winquake.res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\debug_gl/qwcl.bsc"
LINK32=link.exe
# ADD BASE LINK32 ..\common\win32\dxsdk\sdk\lib\dxguid.lib comctl32.lib winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /machine:I386 /nodefaultlib:"libcmtd.lib" /out:".\release_gl/GLQW2.EXE"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 ..\common\win32\dxsdk\sdk\lib\dxguid.lib comctl32.lib winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /map:".\debug_gl/GLQW2.map" /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:".\debug_gl/GLQW2.EXE"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "qwcl - Win32 Release"
# Name "qwcl - Win32 Debug"
# Name "qwcl - Win32 GL QW"
# Name "qwcl - Win32 GL QW Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Group "Gamespy"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\Goa\CEngine\darray.c
# End Source File
# Begin Source File

SOURCE=..\common\Goa\CEngine\darray.h
# End Source File
# Begin Source File

SOURCE=..\common\Goa\CEngine\goaceng.h
# End Source File
# Begin Source File

SOURCE=..\common\Goa\Sdk\goautil.h
# End Source File
# Begin Source File

SOURCE=..\common\Goa\CEngine\gserver.c
# End Source File
# Begin Source File

SOURCE=..\common\Goa\CEngine\gserver.h
# End Source File
# Begin Source File

SOURCE=..\common\Goa\CEngine\gserverlist.c
# End Source File
# Begin Source File

SOURCE=..\common\Goa\CEngine\gutil.c
# End Source File
# Begin Source File

SOURCE=..\common\Goa\CEngine\gutil.h
# End Source File
# Begin Source File

SOURCE=..\common\Goa\CEngine\hashtable.c
# End Source File
# Begin Source File

SOURCE=..\common\Goa\CEngine\hashtable.h
# End Source File
# Begin Source File

SOURCE=..\common\Goa\nonport.c
# End Source File
# Begin Source File

SOURCE=..\common\Goa\nonport.h
# End Source File
# End Group
# Begin Group "DOS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\dos_v2.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\in_dos.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\net_udp.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\sys_dos.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vid_dos.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vid_ext.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vregset.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "GL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gl_draw.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_mesh.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\gl_model.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\gl_model.h

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\gl_ngraph.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_refrag.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_rlight.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_rmain.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_rmisc.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_rsurf.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\gl_screen.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_test.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_vidnt.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_warp.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\gl_warp_sin.h

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\glquake.h

!IF  "$(CFG)" == "qwcl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Software"

# PROP Default_Filter ""
# Begin Group "ASM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\d_draw.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_draw.s
InputName=d_draw

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\d_draw.s
InputName=d_draw

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_draw16.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_draw16.s
InputName=d_draw16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\d_draw16.s
InputName=d_draw16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_parta.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_parta.s
InputName=d_parta

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\d_parta.s
InputName=d_parta

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_polysa.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_polysa.s
InputName=d_polysa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\d_polysa.s
InputName=d_polysa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_scana.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_scana.s
InputName=d_scana

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\d_scana.s
InputName=d_scana

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_spr8.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_spr8.s
InputName=d_spr8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\d_spr8.s
InputName=d_spr8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_varsa.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\d_varsa.s
InputName=d_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\d_varsa.s
InputName=d_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_aclipa.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\r_aclipa.s
InputName=r_aclipa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\r_aclipa.s
InputName=r_aclipa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_aliasa.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\r_aliasa.s
InputName=r_aliasa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\r_aliasa.s
InputName=r_aliasa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_drawa.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\r_drawa.s
InputName=r_drawa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\r_drawa.s
InputName=r_drawa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_edgea.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\r_edgea.s
InputName=r_edgea

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\r_edgea.s
InputName=r_edgea

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_varsa.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\r_varsa.s
InputName=r_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\r_varsa.s
InputName=r_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\surf16.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\surf16.s
InputName=surf16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\surf16.s
InputName=surf16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\surf8.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\surf8.s
InputName=surf8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=..\common\surf8.s
InputName=surf8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\common\d_edge.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_fill.c
# End Source File
# Begin Source File

SOURCE=..\common\d_init.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_modech.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_part.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_polyse.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_scan.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_sky.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_sprite.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_surf.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_vars.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\d_zpoint.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\draw.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\model.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_aclip.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_alias.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_bsp.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_draw.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_edge.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_efrag.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_light.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\r_main.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\r_misc.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_sky.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_sprite.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_surf.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\r_vars.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\screen.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\vid_win.c

!IF  "$(CFG)" == "qwcl - Win32 Release"

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\common\cd_win.c
# End Source File
# Begin Source File

SOURCE=.\cl_cam.c
# End Source File
# Begin Source File

SOURCE=.\cl_demo.c
# End Source File
# Begin Source File

SOURCE=.\cl_ents.c
# End Source File
# Begin Source File

SOURCE=.\cl_http.c
# End Source File
# Begin Source File

SOURCE=.\cl_input.c
# End Source File
# Begin Source File

SOURCE=.\cl_main.c
# End Source File
# Begin Source File

SOURCE=.\cl_parse.c
# End Source File
# Begin Source File

SOURCE=.\cl_pred.c
# End Source File
# Begin Source File

SOURCE=..\common\cl_tent.c
# End Source File
# Begin Source File

SOURCE=.\cmd.c
# End Source File
# Begin Source File

SOURCE=.\common.c
# End Source File
# Begin Source File

SOURCE=..\common\console.c
# End Source File
# Begin Source File

SOURCE=..\common\crc.c
# End Source File
# Begin Source File

SOURCE=..\common\cvar.c
# End Source File
# Begin Source File

SOURCE=..\common\dstring.c
# End Source File
# Begin Source File

SOURCE=..\common\in_win.c
# End Source File
# Begin Source File

SOURCE=..\common\keys.c
# End Source File
# Begin Source File

SOURCE=..\common\math.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build - GAS2MASM Conversion
OutDir=.\Debug
InputPath=..\common\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# Begin Custom Build
OutDir=.\release_gl
InputPath=..\common\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# Begin Custom Build
OutDir=.\debug_gl
InputPath=..\common\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\mathlib.c
# End Source File
# Begin Source File

SOURCE=.\md4.c
# End Source File
# Begin Source File

SOURCE=.\menu.c
# End Source File
# Begin Source File

SOURCE=.\net_chan.c
# End Source File
# Begin Source File

SOURCE=.\net_wins.c
# End Source File
# Begin Source File

SOURCE=..\common\nonintel.c
# End Source File
# Begin Source File

SOURCE=.\pmove.c
# End Source File
# Begin Source File

SOURCE=.\pmovetst.c
# End Source File
# Begin Source File

SOURCE=.\r_part.c
# End Source File
# Begin Source File

SOURCE=.\sbar.c
# End Source File
# Begin Source File

SOURCE=.\skin.c
# End Source File
# Begin Source File

SOURCE=..\common\snd_dma.c
# End Source File
# Begin Source File

SOURCE=..\common\snd_mem.c
# End Source File
# Begin Source File

SOURCE=..\common\snd_mix.c
# End Source File
# Begin Source File

SOURCE=..\common\snd_mixa.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build - GAS2MASM Conversion
OutDir=.\Debug
InputPath=..\common\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# Begin Custom Build
OutDir=.\release_gl
InputPath=..\common\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# Begin Custom Build
OutDir=.\debug_gl
InputPath=..\common\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\snd_strm.c
# End Source File
# Begin Source File

SOURCE=..\common\snd_wavstream.c
# End Source File
# Begin Source File

SOURCE=..\common\snd_win.c
# End Source File
# Begin Source File

SOURCE=..\common\sys_find.c
# End Source File
# Begin Source File

SOURCE=..\common\sys_win.c
# End Source File
# Begin Source File

SOURCE=..\common\sys_wina.s

!IF  "$(CFG)" == "qwcl - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=..\common\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 Debug"

# Begin Custom Build - GAS2MASM Conversion
OutDir=.\Debug
InputPath=..\common\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW"

# Begin Custom Build
OutDir=.\release_gl
InputPath=..\common\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "qwcl - Win32 GL QW Debug"

# Begin Custom Build
OutDir=.\debug_gl
InputPath=..\common\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm.exe < $(OUTDIR)\$(InputName).spp          >$(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                                           $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\view.c
# End Source File
# Begin Source File

SOURCE=..\common\wad.c
# End Source File
# Begin Source File

SOURCE=.\winquake.rc
# End Source File
# Begin Source File

SOURCE=..\common\zone.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\common\adivtab.h
# End Source File
# Begin Source File

SOURCE=..\common\anorms.h
# End Source File
# Begin Source File

SOURCE=.\bothdefs.h
# End Source File
# Begin Source File

SOURCE=.\bspfile.h
# End Source File
# Begin Source File

SOURCE=..\common\cdaudio.h
# End Source File
# Begin Source File

SOURCE=.\cl_http.h
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\cmd.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=..\common\console.h
# End Source File
# Begin Source File

SOURCE=..\common\crc.h
# End Source File
# Begin Source File

SOURCE=..\common\cvar.h
# End Source File
# Begin Source File

SOURCE=..\common\d_iface.h
# End Source File
# Begin Source File

SOURCE=..\common\d_ifacea.h
# End Source File
# Begin Source File

SOURCE=..\common\d_local.h
# End Source File
# Begin Source File

SOURCE=.\draw.h
# End Source File
# Begin Source File

SOURCE=..\common\dstring.h
# End Source File
# Begin Source File

SOURCE=..\common\gl_func.h
# End Source File
# Begin Source File

SOURCE=..\common\input.h
# End Source File
# Begin Source File

SOURCE=..\common\keys.h
# End Source File
# Begin Source File

SOURCE=..\common\mathlib.h
# End Source File
# Begin Source File

SOURCE=.\menu.h
# End Source File
# Begin Source File

SOURCE=.\model.h
# End Source File
# Begin Source File

SOURCE=..\common\modelgen.h
# End Source File
# Begin Source File

SOURCE=.\net.h
# End Source File
# Begin Source File

SOURCE=.\pmove.h
# End Source File
# Begin Source File

SOURCE=.\protocol.h
# End Source File
# Begin Source File

SOURCE=.\quakedef.h
# End Source File
# Begin Source File

SOURCE=..\common\r_local.h
# End Source File
# Begin Source File

SOURCE=..\common\r_shared.h
# End Source File
# Begin Source File

SOURCE=.\render.h
# End Source File
# Begin Source File

SOURCE=.\sbar.h
# End Source File
# Begin Source File

SOURCE=..\common\screen.h
# End Source File
# Begin Source File

SOURCE=..\common\snd_ogg.h
# End Source File
# Begin Source File

SOURCE=..\common\sound.h
# End Source File
# Begin Source File

SOURCE=..\common\spritegn.h
# End Source File
# Begin Source File

SOURCE=..\common\sys.h
# End Source File
# Begin Source File

SOURCE=.\vid.h
# End Source File
# Begin Source File

SOURCE=.\view.h
# End Source File
# Begin Source File

SOURCE=..\common\wad.h
# End Source File
# Begin Source File

SOURCE=..\common\winquake.h
# End Source File
# Begin Source File

SOURCE=..\common\zone.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\qe3.ico
# End Source File
# Begin Source File

SOURCE=.\quakeworld.bmp
# End Source File
# Begin Source File

SOURCE=.\qwcl2.ico
# End Source File
# End Group
# Begin Source File

SOURCE=..\common\win32\libogg.lib
# End Source File
# Begin Source File

SOURCE=..\common\win32\libvorbis.lib
# End Source File
# Begin Source File

SOURCE=..\common\win32\libcurl.lib
# End Source File
# End Target
# End Project
