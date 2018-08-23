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

static const char name[]    = "test10";
static const char purpose[] = "culling test - render a red and blue"
                              " triangle but cull one out - red positive"
                              ", blue negative";
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
    FxU8                 subframe = 0;

    GrCullMode_t cullMode;

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
            frames = 2;
            strcpy(filename, remArgs[0]);
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
    assert( grSstWinOpen(tlGethWnd(),
                         resolution,
                         GR_REFRESH_60Hz,
                         GR_COLORFORMAT_ABGR,
                         GR_ORIGIN_UPPER_LEFT,
                         2, 1 ) );
    
    tlConSet( 0.0f, 0.0f, 1.0f, 1.0f, 
              60, 30, 0xffffff );

    /* Set up Render State */
    tlVertexLayout();

    grColorCombine( GR_COMBINE_FUNCTION_LOCAL,
                    GR_COMBINE_FACTOR_NONE,
                    GR_COMBINE_LOCAL_CONSTANT,
                    GR_COMBINE_OTHER_NONE,
                    FXFALSE );
    cullMode = GR_CULL_POSITIVE;
    grCullMode( cullMode );

    tlConOutput( "Press <SPACE> to toggle cull orientation\n" );
    tlConOutput( "Press any other key to quit\n" );
    while( frames-- && tlOkToRender()) {
        GrVertex vtxA, vtxB, vtxC;

        if (hwconfig == TL_VOODOORUSH) {
          tlGetDimsByConst(resolution,
                           &scrWidth, 
                           &scrHeight );
        
          grClipWindow(0, 0, (FxU32) scrWidth, (FxU32) scrHeight);
        }

        switch( cullMode ) {
        case GR_CULL_POSITIVE:
            tlConOutput( "CULL POSITIVE\r" );
            break;
        case GR_CULL_NEGATIVE:
            tlConOutput( "CULL NEGATIVE\r" );
            break;
        }

          grBufferClear( 0x00, 0, 0 );

        vtxA.x = tlScaleX( 0.5f ), vtxA.y = tlScaleY( 0.1f );
        vtxB.x = tlScaleX( 0.8f ), vtxB.y = tlScaleY( 0.9f );
        vtxC.x = tlScaleX( 0.2f ), vtxC.y = tlScaleY( 0.9f );

        grConstantColorValue( 0x000000ff );
        grDrawTriangle( &vtxA, &vtxB, &vtxC );

        grSstOrigin( GR_ORIGIN_LOWER_LEFT );

        grConstantColorValue( 0x00ff0000 );
        grDrawTriangle( &vtxA, &vtxC, &vtxB );

        grSstOrigin( GR_ORIGIN_UPPER_LEFT );

        tlConRender();
        grBufferSwap( 1 );

        /* grab the frame buffer */
        if (scrgrab) {
          char fname[256], tmp[32];
          FxU16 cnt;

          cnt = strcspn(filename, ".");
          strncpy(fname, filename, cnt);
          fname[cnt] = 0;
          sprintf(tmp,"_%d", subframe);
          strcat(fname, tmp);
          strcat(fname, filename+cnt);
          if (!tlScreenDump(fname, (FxU16)scrWidth, (FxU16)scrHeight))
            printf( "Cannot open %s\n", filename);

          /* cycle through all mode */
          if ( cullMode == GR_CULL_POSITIVE )
            cullMode = GR_CULL_NEGATIVE;
          else 
            cullMode = GR_CULL_POSITIVE;
          grCullMode( cullMode );

          subframe++;
        }

        while( tlKbHit() ) {
            switch( tlGetCH() ) {
            case ' ':
                if ( cullMode == GR_CULL_POSITIVE )
                    cullMode = GR_CULL_NEGATIVE;
                else 
                    cullMode = GR_CULL_POSITIVE;
                grCullMode( cullMode );
                break;
            default:
                frames = 0;
                break;
            }
        }
    }
    
    grGlideShutdown();
    return 0;
}





