Setup:
* Pick the correct glide driver (glide3x.dxe) for your 3dfx card from
  one of the directories listed below and put it in the same place as
  qdosfx.exe, qwdosfx.exe, and gl.dxe:
  - sst1  -> Voodoo Graphics
  - sst96 -> Voodoo Rush
  - cvg   -> Voodoo2
  - h5    -> Banshee and Voodoo 3/4/5
* It is recommended to use RayeR's MTRRLFBE.EXE utility
  (http://rayer.g6.cz/programm/mtrrlfbe.exe) to setup a
  write-combining MTRR for the LFB.  This will dramatically increase
  performance on Voodoo Rush and higher cards.

Recommended System Specs:
* Pentium 2 300mhz.  Pentium 3 550mhz or higher for the best experience.
* Voodoo 3.  Older cards will work, but unknown how well they perform in
  faster computers.

Interesting parameters:
* -bpp for setting 15, 16, or 32 bpp on Voodoo 4/5 cards.
* FX_GLIDE_2PPC controls the 2 Pixels Per Clock mode.  Valid only for
  Voodoo 4/5 cards.  Disabled in multi-texturing mode.  Valid values are
  -1 to disable, 0 driver decides, 1 forced on.  Default is 1.
* FX_GLIDE_LOD_BIAS sets the level detail.  Valid only for Voodoo 3/4/5 cards.
  This one was kind of tricky to figure out, but basically if you want say
  +0.5 you would set the value to 5, for +1.2 you'd set it to 12 and so on.
  Valid ranges are -32 to 32.
* FX_GLIDE_NO_SPLASH=1 to disable the splash logo on Voodoo 1 and 2 cards
  on startup.
* FX_GLIDE_SWAPPENDINGCOUNT to set the number of frames to buffer.  Default
  value is 2, but can go as high as 6.  Traditionally, in Windows, higher
  values can be a slight speed advantage but can introduce input lag.  On
  the hardware I tested it on I was able to gain a few extra frames in
  timedemos (about 3-4) with 6 with no input lag and I am even able to
  use m_filter in combination with this.
* SSTH3_ALPHADITHERMODE set to 3 for the "Smoother" option that is equivalent
  in the Windows control panel for Alpha Blending Quality.  Values 0-2 all
  use the same code path.  Defaults to 1.
* SSTH3_SLI_AA_CONFIGURATION to control SLI on Voodoo 5 5500 as well as
  anti-aliasing on these cards.
  - For Voodoo 4 and 5:
    0 - SLI Disabled, AA Disabled.
    1 - SLI Disabled, AA 2x Enabled.
  - For Voodoo 5 5500:
    2 - SLI Enabled, AA Disabled (default setting).
    3 - SLI Enabled, AA 2x Enabled.
    4 - SLI Disabled, AA 4x Enabled.
  Options 5-8 are for Voodoo 5 6000 users: if you're out there, send me
  an email!
* MESA_FX_IGNORE_CMBEXT to allow Voodoo 4/5 to perform single-pass
  trilinear.  This also provides a small speed boost of 4-5 fps on average
  in my timedemo tests with bilinear.  Mesa warns some advanced (multi)texturing
  modes won't work (GL_EXT_texture_env_combine), but multitexturing is slower
  in Mesa and is recommended to be disabled (see below).

Known Issues:
* Mesa is slower than MiniGL drivers such as the WickedGL driver or the
  OpenGL ICD from 3dfx.  These DLLs rely on special Quake engine hacks
  to speed up rendering.  They are closed source so we do not know what
  tricks were used.  However, Mesa in DOS is comparable in speeds to
  Mesa DLLs for Windows.
* Outdoor scenes are slower compared to Windows OpenGL ICD.  This issue
  also exists in Mesa.  You can verify by downloading the Windows Mesa DLLs
  from http://falconfly.de/ .
* Multitexture is supported by the driver but appears to have an
  approximately 10fps loss on average (more in complex scenes) on everything
  I tested it on EXCEPT in modes 1280x1024 and 1600x1200 where it really
  helps.  It is disabled by default.  If you would like to try it anyways
  start the game with -mtex passed at the command line.  e.g.
  "qdosfx.exe -metx".
* Trilinear filtering is disabled by default on Voodoo 4/5 unless you use
  MESA_FX_IGNORE_CMBEXT.

Other tidbits:
* 3DFX's reference hardware for benchmarking with the VSA-100 chipset
  (Voodoo 4 and 5) was a Pentium 3 866mhz.  A setup in this range provides
  the smoothest framerates and is recommended to play the game comfortably.
  See http://www.falconfly.de/downloads/3dfx-v5-PC-Reviewers-Guide.zip for
  more details.
