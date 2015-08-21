# Microsoft Developer Studio Project File - Name="winquake" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=winquake - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WinQuake.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinQuake.mak" CFG="winquake - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "winquake - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "winquake - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "winquake - Win32 GL Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "winquake - Win32 Release"

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
# ADD CPP /nologo /G5 /MT /GX /I ".\scitech\include" /I ".\dxsdk\sdk\inc" /I ".\winquake" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OGG_SUPPORT" /D "BSP2_SUPPORT" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 .\dxsdk\sdk\lib\dxguid.lib .\scitech\lib\win32\vc\mgllt.lib winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmtd.lib" /out:".\Release/WQDOS.exe"
# SUBTRACT LINK32 /map /debug

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

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
# ADD CPP /nologo /G5 /MTd /GX /ZI /Od /I ".\scitech\include" /I ".\dxsdk\sdk\inc" /I ".\winquake" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "BSP2_SUPPORT" /D "OGG_SUPPORT" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 .\dxsdk\sdk\lib\dxguid.lib .\scitech\lib\win32\vc\mgllt.lib winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /out:".\Debug/DWQDOS.exe"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\winquak0"
# PROP BASE Intermediate_Dir ".\winquak0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\release_gl"
# PROP Intermediate_Dir ".\release_gl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /GX /Ox /Ot /Ow /I ".\scitech\include" /I ".\dxsdk\sdk\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# SUBTRACT BASE CPP /Oa /Og
# ADD CPP /nologo /G5 /GX /Ot /Ow /I ".\dxsdk\sdk\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "GLQUAKE" /D "BSP2_SUPPORT" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib .\scitech\lib\win32\vc\mgllt.lib /nologo /subsystem:windows /profile /machine:I386
# SUBTRACT BASE LINK32 /map /debug
# ADD LINK32 .\dxsdk\sdk\lib\dxguid.lib comctl32.lib winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /machine:I386 /out:".\release_gl\glquake2.exe"
# SUBTRACT LINK32 /map /debug

!ENDIF 

# Begin Target

# Name "winquake - Win32 Release"
# Name "winquake - Win32 Debug"
# Name "winquake - Win32 GL Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Group "Gamespy"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Goa\CEngine\darray.c
# End Source File
# Begin Source File

SOURCE=.\Goa\CEngine\darray.h
# End Source File
# Begin Source File

SOURCE=.\Goa\CEngine\goaceng.h
# End Source File
# Begin Source File

SOURCE=.\Goa\SDK\goautil.c
# End Source File
# Begin Source File

SOURCE=.\Goa\SDK\goautil.h
# End Source File
# Begin Source File

SOURCE=.\Goa\CEngine\gserver.c
# End Source File
# Begin Source File

SOURCE=.\Goa\CEngine\gserver.h
# End Source File
# Begin Source File

SOURCE=.\Goa\CEngine\gserverlist.c
# End Source File
# Begin Source File

SOURCE=.\Goa\CEngine\gutil.h
# End Source File
# Begin Source File

SOURCE=.\Goa\CEngine\hashtable.c
# End Source File
# Begin Source File

SOURCE=.\Goa\CEngine\hashtable.h
# End Source File
# Begin Source File

SOURCE=.\Goa\nonport.c
# End Source File
# Begin Source File

SOURCE=.\Goa\nonport.h
# End Source File
# End Group
# Begin Group "GL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gl_draw.c
# End Source File
# Begin Source File

SOURCE=.\gl_mesh.c
# End Source File
# Begin Source File

SOURCE=.\gl_model.c
# End Source File
# Begin Source File

SOURCE=.\gl_model.h
# End Source File
# Begin Source File

SOURCE=.\gl_refrag.c
# End Source File
# Begin Source File

SOURCE=.\gl_rlight.c
# End Source File
# Begin Source File

SOURCE=.\gl_rmain.c
# End Source File
# Begin Source File

SOURCE=.\gl_rmisc.c
# End Source File
# Begin Source File

SOURCE=.\gl_rsurf.c
# End Source File
# Begin Source File

SOURCE=.\gl_screen.c
# End Source File
# Begin Source File

SOURCE=.\gl_test.c
# End Source File
# Begin Source File

SOURCE=.\gl_vidnt.c
# End Source File
# Begin Source File

SOURCE=.\gl_warp.c
# End Source File
# Begin Source File

SOURCE=.\gl_warp_sin.h
# End Source File
# Begin Source File

SOURCE=.\glquake.h
# End Source File
# Begin Source File

SOURCE=.\glquake2.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cd_win.c
# End Source File
# Begin Source File

SOURCE=.\cfgfile.c
# End Source File
# Begin Source File

SOURCE=.\chase.c
# End Source File
# Begin Source File

SOURCE=.\cl_demo.c
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

SOURCE=.\cl_tent.c
# End Source File
# Begin Source File

SOURCE=.\cmd.c
# End Source File
# Begin Source File

SOURCE=.\common.c
# End Source File
# Begin Source File

SOURCE=.\conproc.c
# End Source File
# Begin Source File

SOURCE=.\console.c
# End Source File
# Begin Source File

SOURCE=.\crc.c
# End Source File
# Begin Source File

SOURCE=.\cvar.c
# End Source File
# Begin Source File

SOURCE=.\dstring.c
# End Source File
# Begin Source File

SOURCE=.\host.c
# End Source File
# Begin Source File

SOURCE=.\host_cmd.c
# End Source File
# Begin Source File

SOURCE=.\in_win.c
# End Source File
# Begin Source File

SOURCE=.\keys.c
# End Source File
# Begin Source File

SOURCE=.\math.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=.\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=.\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# Begin Custom Build
OutDir=.\release_gl
InputPath=.\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mathlib.c
# End Source File
# Begin Source File

SOURCE=.\menu.c
# End Source File
# Begin Source File

SOURCE=.\net_dgrm.c
# End Source File
# Begin Source File

SOURCE=.\net_loop.c
# End Source File
# Begin Source File

SOURCE=.\net_main.c
# End Source File
# Begin Source File

SOURCE=.\net_win.c
# End Source File
# Begin Source File

SOURCE=.\net_wins.c
# End Source File
# Begin Source File

SOURCE=.\net_wipx.c
# End Source File
# Begin Source File

SOURCE=.\pr_cmds.c
# End Source File
# Begin Source File

SOURCE=.\pr_edict.c
# End Source File
# Begin Source File

SOURCE=.\pr_exec.c
# End Source File
# Begin Source File

SOURCE=.\r_part.c
# End Source File
# Begin Source File

SOURCE=.\sbar.c
# End Source File
# Begin Source File

SOURCE=.\snd_dma.c
# End Source File
# Begin Source File

SOURCE=.\snd_mem.c
# End Source File
# Begin Source File

SOURCE=.\snd_mix.c
# End Source File
# Begin Source File

SOURCE=.\snd_mixa.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=.\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=.\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# Begin Custom Build
OutDir=.\release_gl
InputPath=.\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\snd_strm.c
# End Source File
# Begin Source File

SOURCE=.\snd_wavstream.c
# End Source File
# Begin Source File

SOURCE=.\snd_win.c
# End Source File
# Begin Source File

SOURCE=.\sv_main.c
# End Source File
# Begin Source File

SOURCE=.\sv_move.c
# End Source File
# Begin Source File

SOURCE=.\sv_phys.c
# End Source File
# Begin Source File

SOURCE=.\sv_user.c
# End Source File
# Begin Source File

SOURCE=.\sys_find.c
# End Source File
# Begin Source File

SOURCE=.\sys_win.c
# End Source File
# Begin Source File

SOURCE=.\sys_wina.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=.\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=.\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# Begin Custom Build
OutDir=.\release_gl
InputPath=.\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\view.c
# End Source File
# Begin Source File

SOURCE=.\wad.c
# End Source File
# Begin Source File

SOURCE=.\winquake.rc
# End Source File
# Begin Source File

SOURCE=.\world.c
# End Source File
# Begin Source File

SOURCE=.\worlda.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=.\worlda.s
InputName=worlda

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=.\worlda.s
InputName=worlda

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zone.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\anorm_dots.h
# End Source File
# Begin Source File

SOURCE=.\anorms.h
# End Source File
# Begin Source File

SOURCE=.\bspfile.h
# End Source File
# Begin Source File

SOURCE=.\cdaudio.h
# End Source File
# Begin Source File

SOURCE=.\cfgfile.h
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

SOURCE=.\conproc.h
# End Source File
# Begin Source File

SOURCE=.\console.h
# End Source File
# Begin Source File

SOURCE=.\crc.h
# End Source File
# Begin Source File

SOURCE=.\cvar.h
# End Source File
# Begin Source File

SOURCE=.\d_iface.h
# End Source File
# Begin Source File

SOURCE=.\dg_misc.h
# End Source File
# Begin Source File

SOURCE=.\dosisms.h
# End Source File
# Begin Source File

SOURCE=.\draw.h
# End Source File
# Begin Source File

SOURCE=.\dstring.h
# End Source File
# Begin Source File

SOURCE=.\input.h
# End Source File
# Begin Source File

SOURCE=.\keys.h
# End Source File
# Begin Source File

SOURCE=.\mathlib.h
# End Source File
# Begin Source File

SOURCE=.\menu.h
# End Source File
# Begin Source File

SOURCE=.\model.h
# End Source File
# Begin Source File

SOURCE=.\modelgen.h
# End Source File
# Begin Source File

SOURCE=.\net.h
# End Source File
# Begin Source File

SOURCE=.\net_dgrm.h
# End Source File
# Begin Source File

SOURCE=.\net_loop.h
# End Source File
# Begin Source File

SOURCE=.\net_ser.h
# End Source File
# Begin Source File

SOURCE=.\net_wins.h
# End Source File
# Begin Source File

SOURCE=.\net_wipx.h
# End Source File
# Begin Source File

SOURCE=.\pr_comp.h
# End Source File
# Begin Source File

SOURCE=.\progdefs.h
# End Source File
# Begin Source File

SOURCE=.\progs.h
# End Source File
# Begin Source File

SOURCE=.\protocol.h
# End Source File
# Begin Source File

SOURCE=.\quakedef.h
# End Source File
# Begin Source File

SOURCE=.\r_local.h
# End Source File
# Begin Source File

SOURCE=.\r_shared.h
# End Source File
# Begin Source File

SOURCE=.\render.h
# End Source File
# Begin Source File

SOURCE=.\sbar.h
# End Source File
# Begin Source File

SOURCE=.\screen.h
# End Source File
# Begin Source File

SOURCE=.\server.h
# End Source File
# Begin Source File

SOURCE=.\snd_ogg.h
# End Source File
# Begin Source File

SOURCE=.\sound.h
# End Source File
# Begin Source File

SOURCE=.\spritegn.h
# End Source File
# Begin Source File

SOURCE=.\sys.h
# End Source File
# Begin Source File

SOURCE=.\vid.h
# End Source File
# Begin Source File

SOURCE=.\view.h
# End Source File
# Begin Source File

SOURCE=.\wad.h
# End Source File
# Begin Source File

SOURCE=.\winquake.h
# End Source File
# Begin Source File

SOURCE=.\world.h
# End Source File
# Begin Source File

SOURCE=.\zone.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\quake.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\progdefs.q1
# End Source File
# Begin Source File

SOURCE=.\progdefs.q2
# End Source File
# Begin Source File

SOURCE=..\win32\libogg.lib
# End Source File
# Begin Source File

SOURCE=..\win32\libvorbis.lib
# End Source File
# End Target
# End Project
