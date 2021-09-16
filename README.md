Quake v1.09 and QuakeWorld v2.40 for DOS with WATTCP Stack
Original Concept and WATTCP import coding by NeoZeed
Coding by [HCI]Mara'akate and sezero

Additional code from uHexen2, taniwha, spoike, LordHavoc, 
   QuakeForge, Fitzquake, QuakeSpasm, Darkplaces and more.
---------------------------------------------------------

Table of Contents

1. Introduction
2. What's New
3. Requirements
4. Installation
5. Known Bugs and Limitations
6. Final Thoughts


1. - Introduction
---------------------------------------------------------

Why code for DOS in 2015 you ask?  Pretty much the same reason anybody
   is coding Quake ports these days... for fun and to learn some programming.
   This is my first serious project as far as programming goes and I have
   learned a lot.  As each day goes by, QDOS and QWDOS grow more and more in
   their complexity with new features and bug fixes.  I also love knowing
   all that is sitting between you and Quake is a packet driver, a mouse
   driver and pure DOS. =)

2. - What's New
---------------------------------------------------------

QDOS has many features, I can't remember each one that me or others have added
   but are some highlights:

* External *.ent file support, and entity dumping in-game.   
* Fitzquake v666 Protocol support.  Thanks to Fitzquake.
* Warp Spasm and other large mods support.  Thanks to Fitzquake.
* WATT32 -- Allows you to play QDOS online with a simple
   packet driver and WATTCP.CFG.  Thanks to NeoZeed.
* BSP Version 28 support (Quake Pre-Release).  Thanks to Engoo engine guy.
* -NOGUS parameter to disable GUS init.  Useful if you have an SB16
   and GUS co-existing.
* -NOCD parameter does the same as -NOCDAUDIO, but is quicker to type.
* -SAFEVGA paramater to for 320x200 mode.
* -QUOTH paramater for mods that required Quoth.
* Dynamic strings for most vars.  This should help with buffer oveflows.
   Thanks to QuakeForge crew for helping me implement it.
* Press jump in NOCLIP/Water to move up.
* F_VERSION reply.  Thanks to QuakeForge crew.
* Proper timer.  Huge thanks to mysterious DJGPP guru.
* Various bug fixes that have been recommended by QuakeForge, LordHavoc,
   Sezero, and other engine authors.
* Chat buffer --  Press Up/Down or Left/Right arrows to scroll through
   previously sent chat messages.  Thanks to Taniwha for more efficient
   code.
* Gun model is drawn if FOV > 90.
* Gamespy Server Browser accessible from Multiplayer Menu and with slist2
  command.  Adapted from Q2DOS.
* BSP2 and 2PSB support. Code (with some slight readaptations) from QuakeSpasm.
  Additonal help from sezero.  These maps may need things like:
  r_maxedges and r_maxsurfs set to very high values.  Try setting both
  to 25000 and restarting the map if you see disappearing objects as you
  look around.
* WAV and Ogg Vorbis streaming music support.  Put tracks in 
  id1/music/trackXX format.
* Some FitzQuake extensions in QWDOS, like extended limits and chunked downloads.
* HTTP downloading via CURL in QWDOS for QuakeForge servers.
* -cddir for specifiy a cd drive path for pak files.  Also for keeping
  WAV/OGG music in a separate drive.
* 3DFX Voodoo rendering with fxMesa 6.4.2!  Check voodoo directory for 
  your glide3x.dxe.

New CVARs and CMDs:

* CL_NODEMO -- Startup demos are disabled > 1.
* CL_UNBINDALL_PROTECTION -- Protects your CVARs from accidentally being
   cleared from UNBINDALL.  Very useful if you use autocomplete and
   accidentally end up on this CMD!
* CL_WARNCMD -- From QukeWorld, warn if a command is invalid.
* FLASHLIGHT -- Bind this CMD to toggle a dlight around yourself.
* IN_FREELOOK -- Toggle mouse look.  Useful so you don't have to +mlook your
   autoexec.cfg.
* JOY_RECALIBRATE -- CMD to recalibrate the joystick.
* MAX_EDICTS -- Now a CVAR.  Default is 2048.  Make sure you have
   enough RAM allocated!
* PQ_FULLPITCH and CL_FULLPITCH support for ProQuake servers.
* S_KHZ -- Read at startup to change the sampling rate of the audio.
* SHOW_TIME -- 1 for 24hr format, 2 for 12hr AM/PM format.
* SHOW_UPTIME -- 1 for Server/Map uptime, 2 for Client uptime.
* SHOW_FPS -- From QuakeWorld, show Frames Per Second.
* SND_RESTART -- CMD to restart the audio driver.
* SV_ALTNOCLIP -- From FitzQuake, alternate style of NOCLIP.  Defaults to 1.
* SV_DUMPENTITIES -- Dump entities of current map to /maps gamedir.
* SV_LOADENTFILES -- Load external *.ent files.  Defaults to 1.
* SV_PROTOCOL -- 15 for original Quake protocol, 666 for Fitzquake
   protocol.  666 is default.
* V_CONTENTBLEND -- This function is like gl_polyblend.  Set to 0
   to disable powerup and damage flashes, etc.

3. - Requirements
---------------------------------------------------------

Because QDOS has supports for extended limits and other new features, the
system requirements are slightly higher compared to original Quake.

If you're going to simply play on QUAKE.SHMACK.NET or stock single player
the requirements are the same as original Quake.

However, to take advantage of the Fitzquake protocol and extended limits it
is required to have at least 64MB of RAM.  Some mods may require more.
I'd highly recommend allocating at least 96MB if your system can
support this.  A Pentium 2 processor is also highly recommended.

To play at extreme SVGA resolutions with acceptable (i.e. >= 40FPS) framerates a
P3 ~800MHZ processor or higher is recommended.

Online play over TCP/IP will require a NIC with a Packet Driver and a
properly configured WATTCP.CFG in your root /QUAKE directory.  IPX and
Serial/Modem drivers are still supported.

Do not send bug reports on any bugs that only occur on Windows 9x.  They
will be ignored!

NOTE: QDOS tries to allocate 32MB by default so you may need to use -MEM 16 if
you're on an older machine and trying to play stock Quake.

To summarize:

**Stock Quake Play:**

  * FPU Processor
  * 16MB RAM
  * VGA Videocard
  * MS-DOS v6.00
  * Keyboard, Mouse, or Joystick
  * Sound Blaster compatible or Gravis UltraSound

**Extended Limits:**

  * Pentium 2 (P3 ~800MHZ processor for serious SVGA)
  * 64MB preferred, 96MB strongly recommended.
  * VESA 2.0 videocard.
  * MS-DOS v7.00 (Windows 98 with a properly configured MSDOS.SYS and
    CONFIG.SYS)

4. - Installation
---------------------------------------------------------

1. Extract to your Quake directory
2. Load your Packet Driver and Mouse Driver.
3. Run QDOS.EXE
4. Frag on!

5. - Known Bugs and Limitations
---------------------------------------------------------

* Limited Win9x support.  3DFX Voodoo builds will not work in Windows.
* Some of the health and ammo boxes in Warp Spasm clip improperly.
  From what I have read, it appears to be related to a QC issue with
  rotating these at odd angles.
* QDOS Nehahra support is incomplete: the version 32 sprites, the new coloured 
  explosion effects, and probably a few other things I am forgetting are
  currently 'stubbed'.  The game, however, appears to be
  playable despite these limitations.
* Gravis Ultrasound cards may not be initialized properly.  You can start
   ULTRAMID -Q.  Then run, ULTRAMID -F -Q.  This will initialize your card.
   For a batch script that handles this issue please see:
    http://dk.toastednet.org/QDOS/GUSFIX.ZIP

6. - Final Thoughts
---------------------------------------------------------

I highly recommend visiting the Utilities section of
http://dk.toastednet.org/QDOS and using UNIRV and FASTVID to greatly
increase the video speed in Quake under pure DOS.  DOSLFN will also
help avoid file renaming conflicts with long file names.

Special thanks to: neozeed, sezero, Taniwha, LordHavoc, and Spoike.

Questions?  Comments?  Email me emoaddict15@gmail.com
