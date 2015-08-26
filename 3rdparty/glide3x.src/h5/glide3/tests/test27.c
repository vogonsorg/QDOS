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
*/

#include <stdlib.h>
#include <stdio.h>
#ifndef __linux__
#include <conio.h>
#else
#include <linutil.h>
#endif
#include <assert.h>

#include <glide.h>
#include "tlib.h"

int hwconfig;
static const char *version;

static const char name[]    = "test27";
static const char purpose[] = "Cycle the 3D hardware on and off";
static const char usage[]   = "-n <frames> -r <res>";

static int rRandom(int s, int e);
static unsigned int iRandom (unsigned int maxr);

void main( int argc, char **argv) {
    char match; 
    char **remArgs;
    int  rv;

    GrScreenResolution_t resolution = GR_RESOLUTION_640x480;
    float                scrWidth   = 640.0f;
    float                scrHeight  = 480.0f;
    int frames                      = -1;
    int doNothing = 0;

    int cycles;
    FxU32 wrange[2];

    /* Initialize Glide */
    grGlideInit();
    assert( hwconfig = tlVoodooType() );

    /* Process Command Line Arguments */
    while( rv = tlGetOpt( argc, argv, "Nnr", &match, &remArgs ) ) {
        if ( rv == -1 ) {
            printf( "Unrecognized command line argument\n" );
            printf( "%s %s\n", name, usage );
            printf( "Available resolutions:\n%s\n",
                    tlGetResolutionList() );
            return;
        }
        switch( match ) {
        case 'n':
            frames = atoi( remArgs[0] );
            break;
        case 'r':
            resolution = tlGetResolutionConstant( remArgs[0], 
                                                  &scrWidth, 
                                                  &scrHeight );
            break;
        case 'N':
            doNothing = 1;
            break;
        }
    }

    tlSetScreen( scrWidth, scrHeight );

    version = grGetString( GR_VERSION );

    printf( "%s:\n%s\n", name, purpose );
    printf( "%s\n", version );
    printf( "Resolution: %s\n", tlGetResolutionString( resolution ) );
    if ( frames == -1 ) {
        printf( "Press A Key To Begin Test.\n" );
        tlGetCH();
    }
    
    grSstSelect( 0 );


    cycles = 0;
    while( frames-- && tlOkToRender()) {
        GrVertex a, b, c;
        GrContext_t context;
        char inchar;

        context = grSstWinOpen(tlGethWnd(),
                               resolution,
                               GR_REFRESH_60Hz,
                               GR_COLORFORMAT_ABGR,
                               GR_ORIGIN_UPPER_LEFT,
                               2, 1 );
        if (!context) {
          printf("Could not allocate glide fullscreen context.\n");
          goto __errExit;
        }

        /* 
         * Don't like gotos?  In the immortal words of Schwarzenegger
         * (Total Recall): "...so sue me d**khead"
         */
        if (doNothing) goto doNothing;

        grVertexLayout(GR_PARAM_XY,  0, GR_PARAM_ENABLE);
        grVertexLayout(GR_PARAM_RGB, GR_VERTEX_R_OFFSET << 2, GR_PARAM_ENABLE);
        grGet(GR_WDEPTH_MIN_MAX, 8, (FxI32 *)wrange);  

        grColorCombine( GR_COMBINE_FUNCTION_LOCAL,
                        GR_COMBINE_FACTOR_NONE,
                        GR_COMBINE_LOCAL_ITERATED,
                        GR_COMBINE_OTHER_NONE,
                        FXFALSE );

        tlConSet( 0.0f, 0.0f, 1.0f, 1.0f, 
                  60, 30, 0xffffff );
        
        
        /* deal with dynamic resizing */
        if (hwconfig == TL_VOODOORUSH) {
          tlGetDimsByConst(resolution, &scrWidth, &scrHeight);
          grClipWindow(0, 0, (FxU32) scrWidth, (FxU32) scrHeight);
        }

        tlConOutput( "Press <space> to cycle hardware\n" );
        tlConOutput( "Any other key to quit\n" );
        
        grBufferClear( 0x000000, 0, wrange[1] );

        a.r = a.g = a.b = 0.0f;
        b = c = a;

        a.x = tlScaleX(((float)rRandom( 0, 100 ))/100.0f);
        a.y = tlScaleY(((float)rRandom( 0, 100 ))/100.0f);
        a.r = 255.0f;

        b.x = tlScaleX(((float)rRandom( 0, 100 ))/100.0f);
        b.y = tlScaleY(((float)rRandom( 0, 100 ))/100.0f);
        b.g = 255.0f;

        c.x = tlScaleX(((float)rRandom( 0, 100 ))/100.0f);
        c.y = tlScaleY(((float)rRandom( 0, 100 ))/100.0f);
        c.b = 255.0f;

        grDrawTriangle( &a, &b, &c );

        tlConOutput( "Cycle: %d\r", cycles );
        tlConRender();
        grBufferSwap( 1 );

doNothing:
        inchar = tlGetCH();
        if ( inchar != ' ' ) frames = 0;
        
        grSstWinClose(context);
        cycles++;
    }

 __errExit:    
    grGlideShutdown();
    return;
}

static unsigned long randx = 1;

static unsigned int iRandom (unsigned int maxr)
{
    unsigned int n,retval;

    if (maxr > 0xFFFFFFF) {
        do {
            retval = iRandom(0xFFFFFFF);
            retval |= iRandom(maxr>>28)<<28;
        } while (retval > maxr);
        return retval;
    }
    for (n=1; n<32; n++)
        if (((unsigned)1 << n) > maxr) break;
    do {
        randx = randx*1103515245 + 12345;
        retval = (randx & 0x7fffffff) >> (31-n);
    } while (retval > maxr);
    return retval;
}

static int rRandom(int s, int e)
{
    return s + iRandom(e-s);
}



