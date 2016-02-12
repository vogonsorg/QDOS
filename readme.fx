     .,o'       `o,.
   ,o8'           `8o.
  o8'               `8o
 o8:                 ;8o
.88                   88.
:88.                 ,88:
`888                 888'
 888o   `8888888'   o888
 `888o,. `88888' .,o888'
  `8888888888888888888'
    `888888888888888'
       `::88888;:'
          88888
          88888
          `888'
           `;'

+----------------------------------+
|        Table of Contents         |
|                                  |
|  1 - Setup                       |
|  2 - Minimum System Specs        |
|  3 - Recommend System Specs      |
|  4 - Mesa vs. Sage               |
|  5 - Interesting Paramaters      |
|  6 - Interesting Mesa Parameters |
|  7 - Interesting Sage Parameters |
|  8 - Known General Issues        |
|  9 - Known Mesa Issues           |
| 10 - Known Sage Issues           |
| 11 - Other Tidbits               |
+----------------------------------+

1 - Setup
---------------------------------------------------------
* Pick the correct glide driver (glide3x.dxe) for your 3dfx card from
  one of the directories listed below and put it in the same place as
  qdosfx.exe, qwdosfx.exe:
  - voodoo/sst1  -> Voodoo Graphics
  - voodoo/sst96 -> Voodoo Rush
  - voodoo/cvg   -> Voodoo2
  - voodoo/h5    -> Banshee and Voodoo 3/4/5
* Pick a renderer (gl.dxe) from one of the directories listed below
  and put it in the same place as qdosfx.exe, qwdosfx.exe:
  - opengl/mesa -> Mesa v6.0.2
  - opengl/sage -> Sage (be sure to copy sage.ini as well)
* It is recommended to use RayeR's MTRRLFBE.EXE utility
  (http://rayer.g6.cz/programm/mtrrlfbe.exe) to setup a
  write-combining MTRR for the LFB.  This will dramatically increase
  performance on Voodoo Rush and higher cards.

2 - Minimum Sysem Specs
---------------------------------------------------------
* Pentium 1 with a Voodoo 1.
* A DOS OS capable of detecting more than 64MB of RAM.  DOS Quake locks
  assets on start and locks about 24MB in addition to the default 32MB.
  This may cause out of memory issues and other general oddness on launch.
  DOS 7 from Win98SE is known to work properly.  FreeDOS is assumed to be OK.
  DOS 6 and lower is NOT compatible.

3 - Recommended System Specs
---------------------------------------------------------
* Pentium 2 300mhz.  Pentium 3 550mhz or higher for the best experience.
* Voodoo 2.  Older cards will work, but they don't perform very well.

4 - Mesa vs. Sage
---------------------------------------------------------
* Sage is much faster than Mesa, very close to or in some cases exceeding
  Windows performance compared to the OpenGL ICD by AmigaMerlin v2.9.
  It is not totally perfect, it has it's own share of issues read below
  for more information.
* Mesa has been more thoroughly tested but also has it's own share of issues.
  Read below for more information.

5 - Interesting parameters
---------------------------------------------------------
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
* FX_GLIDE_REFRESH for refresh rate control on Banshee, Voodoo 3, 4, and 5.
* FX_GLIDE_SWAPINTERVAL to control V-Sync.
* FX_GLIDE_SWAPPENDINGCOUNT to set the number of frames to buffer.  Default
  value is 2, but can go as high as 6.  Traditionally, in Windows, higher
  values can be a slight speed advantage but can introduce input lag.  On
  the hardware I tested it on I was able to gain a few extra frames in
  timedemos (about 3-4) with 6 with no input lag and I am even able to
  use m_filter in combination with this.
* SST_SCREENREFRESH for refresh rate control on Voodoo 1, Rush.
* SSTH3_ALPHADITHERMODE set to 3 for the "Smoother" option that is equivalent
  in the Windows control panel for Alpha Blending Quality.  Values 0-2 all
  use the same code path.  Defaults to 1.
* SSTH3_SLI_AA_CONFIGURATION to control SLI on Voodoo 5 5500 as well as
  anti-aliasing on these cards.
  - For Voodoo 4 and 5:
    0 - SLI Disabled, AA Disabled.
    1 - SLI Disabled, 2xAA Enabled. (has issues, see Known General Issues)
  - For Voodoo 5 5500:
    2 - SLI Enabled, AA Disabled (default setting).
    3 - SLI Enabled, 2xAA Enabled. (has issues, see Known General Issues)
    4 - SLI Disabled, 4xAA Enabled.
  Options 5-8 are for Voodoo 5 6000 users: if you're out there, send me
  an email!
* SSTV2_SCREENREFRESH for refresh rate control on Voodoo 2.
* Voodoo 1 users:
  - SST_FASTPCIRD for fast PCI reads.
  - SST_GRXCLK for setting the clock speed.  Mesa Readme states 57 is a safe
    value.
* Voodoo 2 users (Mesa Readme says this will hang SLI and Quantum3D 100SB):
  - SSTV2_FASTMEM_RAS_READS for fast RAS read memory timings.
  - SSTV2_FASTPCIRD for fast PCI reads.
  - SSTV2_GRXCLK for setting the clock speed.  Mesa Readme states 95 is a safe
    value.
* Quantum3D Obsidian3D-2 X-24 users:
  Mesa reports this was necessary for it to work under Linux.
  Let us know if this is necessary for DOS as well:
  - SSTV2_FT_CLKDEL=5
  - SSTV2_TF0_CLKDEL=7
  - SSTV2_TF1_CLKDEL=7
  - SSTV2_TF2_CLKDEL=7
  - SSTV2_SLIM_VIN_CLKDEL=3
  - SSTV2_SLIM_VOUT_CLKDEL=2
  - SSTV2_SLIS_VIN_CLKDEL=3
  - SSTV2_SLIS_VOUT_CLKDEL=2

6 - Interesting Mesa parameters
---------------------------------------------------------
* MESA_FX_IGNORE_CMBEXT to allow Voodoo 4/5 to perform single-pass
  trilinear.  This also provides a small speed boost of 4-5 fps on average
  in my timedemo tests with bilinear.  Mesa warns some advanced (multi)texturing
  modes won't work (GL_EXT_texture_env_combine), but multitexturing is slower
  in Mesa and is recommended to be disabled (see below).
* MESA_FX_IGNORE_TEXFMT set to any value (including 0) to disable the
  32bpp-like quality on 16bpp modes.  This causes a slightly performance hit.
  This is enabled by default, and only affects Voodoo 4 and 5.

7 - Interesting Sage parameters
---------------------------------------------------------
* See sage.ini for more information.

8 - Known General Issues
---------------------------------------------------------
* Multitexture is supported by the driver but appears to have an
  approximately 10fps loss on average (more in complex scenes) on everything
  I tested it on EXCEPT:
  * 15bpp/16bpp 1280x1024 and higher.
  * 32bpp 1024x768 and higher.
  * AA modes (any mode and bpp combo).
  Since most people are likely to be using 16bpp and 640x480 through 1024x768
  it is disabled by default.  If you would like to try it anyways
  start the game with -mtex passed at the command line.  e.g.
  "qdosfx.exe -metx".
* 2xAA (With or without SLI) locks if LFB WC is set.  4xAA is OK.

9 - Known Mesa Issues
---------------------------------------------------------
* Mesa is slower than MiniGL drivers such as the WickedGL driver or the
  OpenGL ICD from 3dfx.  These DLLs rely on special Quake engine hacks
  to speed up rendering.  They are closed source so we do not know what
  tricks were used.  However, Mesa in DOS is comparable in speeds to
  Mesa DLLs for Windows.
* Outdoor scenes are slower compared to Windows OpenGL ICD.  This issue
  also exists in Mesa.  You can verify by downloading the Windows Mesa DLLs
  from http://falconfly.de/ .
* Trilinear filtering is disabled by default on Voodoo 4/5 unless you use
  MESA_FX_IGNORE_CMBEXT.

10 - Known Sage Issues
---------------------------------------------------------
* Trilinear filtering does not work on Voodoo 4 and 5.
* Requires initialization twice for proper performance.  There is a hack in
  our client which does that automatically already until the issue is fixed.

11 - Other Tidbits
---------------------------------------------------------
* 3DFX's reference hardware for benchmarking with the VSA-100 chipset
  (Voodoo 4 and 5) was a Pentium 3 866mhz.  A setup in this range provides
  the smoothest framerates and is recommended to play the game comfortably.
  See http://www.falconfly.de/downloads/3dfx-v5-PC-Reviewers-Guide.zip for
  more details.
* Dynamic lights are a big performance hit on slower computers.  3DFX has no
  hardware tnl and Quake takes no advantage of hardware tnl.  You can either
  disable dynamic lights entirely with gl_dynamic 0, or you can use the fake
  dynamic lighting effect with gl_flashblend 1.  Gl_flashblend 1 enables a
  fake dynamic lighting effect that is basically free (performance) by
  rendering coloured halo effects via OpenGL.  They are not true dynamic
  lights, but can help seeing rocket blasts in deathmatch :).
