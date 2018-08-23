/*
** Insert new header here
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <glide.h>
#include "tlib.h"


int hwconfig;
static const char *version;

static const char name[]    = "test06";
static const char purpose[] = "renders two interpenetrating triangles with w-buffering";
static const char usage[]   = "-n <frames> -r <res> -d <filename>";

int main( int argc, char **argv) {
    char match; 
    char **remArgs;
    int  rv;

    GrScreenResolution_t resolution = GR_RESOLUTION_640x480;
    float                scrWidth   = 640.0f;
    float                scrHeight  = 480.0f;
    int frames                      = -1;
    FxBool               scrgrab = FXFALSE;
    char                 filename[256];
    FxU32                wrange[2];

    /* Initialize Glide */
    grGlideInit();
    assert( hwconfig = tlVoodooType() );

    /* Process Command Line Arguments */
    while ((rv = tlGetOpt(argc, argv, "nrd", &match, &remArgs)) != 0) {
        if ( rv == -1 ) {
            printf( "Unrecognized command line argument\n" );
            printf( "%s %s\n", name, usage );
            printf( "Available resolutions:\n%s\n",
                    tlGetResolutionList() );
            return -1;
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
        case 'd':
            scrgrab = FXTRUE;
            frames = 1;
            strcpy(filename, remArgs[0]);
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
    assert( grSstWinOpen(tlGethWnd(),
                         resolution,
                         GR_REFRESH_60Hz,
                         GR_COLORFORMAT_ABGR,
                         GR_ORIGIN_UPPER_LEFT,
                         2, 1 ) );
    
    grGet(GR_WDEPTH_MIN_MAX, 8, wrange);  

    tlConSet( 0.0f, 0.0f, 1.0f, 1.0f, 
              60, 30, 0xffffff );

    /* Set up Render State - flat shading + w-buffering */
    grVertexLayout(GR_PARAM_XY,  GR_VERTEX_X_OFFSET << 2, GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_Q,   GR_VERTEX_OOW_OFFSET << 2, GR_PARAM_ENABLE);

    grColorCombine( GR_COMBINE_FUNCTION_LOCAL,
                    GR_COMBINE_FACTOR_NONE,
                    GR_COMBINE_LOCAL_CONSTANT,
                    GR_COMBINE_OTHER_NONE,
                    FXFALSE );
    grDepthBufferMode( GR_DEPTHBUFFER_WBUFFER );
    grDepthBufferFunction( GR_CMP_LESS );
    grDepthMask( FXTRUE );

    tlConOutput( "Press a key to quit\n" );
    while( frames-- && tlOkToRender()) {
        GrVertex vtxA, vtxB, vtxC;
        float wDist;

        if (hwconfig == TL_VOODOORUSH) {
          tlGetDimsByConst(resolution,
                           &scrWidth, 
                           &scrHeight );
        
          grClipWindow(0, 0, (FxU32) scrWidth, (FxU32) scrHeight);
        }

        grBufferClear(0x00, 0, wrange[1]);

        vtxA.x = tlScaleX( 0.25f ), vtxA.y = tlScaleY( 0.21f );
        vtxB.x = tlScaleX( 0.75f ), vtxB.y = tlScaleY( 0.21f );
        vtxC.x = tlScaleX( 0.5f  ), vtxC.y = tlScaleY( 0.79f );

        /*----------------------------------------------------------- 
          OOW Values are in the range (1,1/65535)

          This can be the exact same computed 1/W that you use
          for texture mapping.  This saves on host computation and 
          vertex data transferred across the PCI bus.
          -----------------------------------------------------------*/
        wDist = 10.0f;
        vtxA.oow = vtxB.oow = vtxC.oow = ( 1.0f / wDist );

        grConstantColorValue( 0x00808080 );

        grDrawTriangle( &vtxA, &vtxB, &vtxC );


        vtxA.x = tlScaleX( 0.86f ), vtxA.y = tlScaleY( 0.21f );
        vtxB.x = tlScaleX( 0.86f ), vtxB.y = tlScaleY( 0.79f );
        vtxC.x = tlScaleX( 0.14f ), vtxC.y = tlScaleY( 0.5f );

        /*----------------------------------------------------------- 
          OOW Values are in the range (1,1/65535)

          This can be the exact same computed 1/W that you use
          for texture mapping.  This saves on host computation and 
          vertex data transferred across the PCI bus.
          -----------------------------------------------------------*/
        wDist = 12.5f;
        vtxA.oow = vtxB.oow = ( 1.0f / wDist );
        wDist = 7.5f;
        vtxC.oow = ( 1.0f / wDist );

        grConstantColorValue( 0x0000FF00 );

        grDrawTriangle( &vtxA, &vtxB, &vtxC );

        tlConRender();
        grBufferSwap( 1 );

        /* grab the frame buffer */
        if (scrgrab) {
          if (!tlScreenDump(filename, (FxU16)scrWidth, (FxU16)scrHeight))
            printf( "Cannot open %s\n", filename);
          scrgrab = FXFALSE;
        }

        if ( tlKbHit() ) frames = 0;
    }
    
    grGlideShutdown();
    return 0;
}





