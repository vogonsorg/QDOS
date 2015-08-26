 /*
** Insert new header here
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

static const char name[]    = "test24";
static const char purpose[] = "anti-aliased lines test";
static const char usage[]   = "-n <frames> -r <res>";

static int rRandom(int s, int e);
static unsigned int iRandom (unsigned int maxr);

typedef enum { NORMAL, ANTIALIASED } Mode;

void main( int argc, char **argv) {
    char match; 
    char **remArgs;
    int  rv;

    GrScreenResolution_t resolution = GR_RESOLUTION_640x480;
    float                scrWidth   = 640.0f;
    float                scrHeight  = 480.0f;
    int frames                      = -1;

    Mode mode;

    int i;
    static TlVertex3D srcVerts[100];
    float angle;

    int                  ftsize = 0;
    GrFog_t              *fogtable = NULL;
    FxU32                wrange[2];

    /* Initialize Glide */
    grGlideInit();
    assert( hwconfig = tlVoodooType() );

    /* Process Command Line Arguments */
    while( rv = tlGetOpt( argc, argv, "nr", &match, &remArgs ) ) {
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
                         GR_ORIGIN_LOWER_LEFT,
                         2, 1 ) );
    
    tlConSet( 0.0f, 0.0f, 1.0f, 0.5f, 
              60, 15, 0xffffff );
    
    /* Set up Render State - flat shading - alpha blending */
    grVertexLayout(GR_PARAM_XY,  GR_VERTEX_X_OFFSET << 2, GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_A,   GR_VERTEX_A_OFFSET << 2, GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_Q,   GR_VERTEX_OOW_OFFSET << 2, GR_PARAM_ENABLE);

    grGet(GR_WDEPTH_MIN_MAX, 8, wrange);  
    grGet(GR_FOG_TABLE_ENTRIES, 4, &ftsize);
    fogtable = malloc(sizeof(GrFog_t)*ftsize);
    assert(fogtable);

    grColorCombine( GR_COMBINE_FUNCTION_LOCAL,
                    GR_COMBINE_FACTOR_NONE,
                    GR_COMBINE_LOCAL_CONSTANT,
                    GR_COMBINE_OTHER_NONE,
                    FXFALSE );
    grAlphaCombine( GR_COMBINE_FUNCTION_LOCAL,
                    GR_COMBINE_FACTOR_NONE,
                    GR_COMBINE_LOCAL_ITERATED,
                    GR_COMBINE_OTHER_NONE,
                    FXFALSE );
    grFogMode( GR_FOG_WITH_TABLE_ON_W );
    grFogColorValue( 0x0 );
    guFogGenerateExp( fogtable, .9f );
    grFogTable( fogtable );
    grAlphaBlendFunction( GR_BLEND_SRC_ALPHA, GR_BLEND_ONE_MINUS_SRC_ALPHA,
                          GR_BLEND_ZERO, GR_BLEND_ZERO );

    /* Initialize Source 3D data - One Hundred Random Points within
       a 1x1 box */
    for( i = 0; i < 100; i++ ) {
        srcVerts[i].x = (((float)rRandom( 0, 100 )) / 100.0f ) - 0.5f;
        srcVerts[i].y = (((float)rRandom( 0, 100 )) / 100.0f ) - 0.5f;
        srcVerts[i].z = (((float)rRandom( 0, 100 )) / 100.0f ) - 0.5f;
        srcVerts[i].w = 1.0f;
    }

#define RED  0x000000ff
#define BLUE 0x00ff0000

    angle = 0.0f;
    mode = ANTIALIASED;

    tlConOutput( "a - toggles anti-aliasing\n" );
    tlConOutput( "Press any key to quit\n\n" );
    while( frames-- && tlOkToRender()) {
        GrVertex vtxA, vtxB;
        static TlVertex3D xfVerts[100];
        static TlVertex3D prjVerts[100];

        if (hwconfig == TL_VOODOORUSH) {
          tlGetDimsByConst(resolution,
                           &scrWidth, 
                           &scrHeight );
        
          grClipWindow(0, 0, (FxU32) scrWidth, (FxU32) scrHeight);
        }

        grBufferClear( 0x0, 0, wrange[1] );

        /* 3D Transformations */
        angle += 1.0f;
        if ( angle > 359.0f ) angle = 0.0f;

        tlSetMatrix( tlIdentity() );
        tlMultMatrix( tlYRotation( angle ) );
        tlMultMatrix( tlTranslation( 0.0f, 0.0f, 1.3f ) );

        tlTransformVertices( xfVerts, srcVerts, 100 );
        tlProjectVertices( prjVerts, xfVerts, 100 );

        grConstantColorValue( 0xffffffff );

        switch( mode ) {
        case NORMAL:
          tlConOutput( "NORMAL LINES      \r" );
          grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
                         GR_COMBINE_FACTOR_ONE,
                         GR_COMBINE_LOCAL_NONE,
                         GR_COMBINE_OTHER_CONSTANT,
                         FXFALSE);
          grAlphaBlendFunction(GR_BLEND_ONE , GR_BLEND_ZERO, 
                               GR_BLEND_ONE, GR_BLEND_ZERO);
          grDisable(GR_AA_ORDERED);
          grVertexLayout(GR_PARAM_A,
                         GR_VERTEX_A_OFFSET << 2,
                         GR_PARAM_DISABLE);
          break;
        case ANTIALIASED:
          grAlphaCombine( GR_COMBINE_FUNCTION_LOCAL,
                          GR_COMBINE_FACTOR_NONE,
                          GR_COMBINE_LOCAL_ITERATED,
                          GR_COMBINE_OTHER_NONE,
                          FXFALSE );
          grAlphaBlendFunction( GR_BLEND_SRC_ALPHA, GR_BLEND_ONE_MINUS_SRC_ALPHA,
                                GR_BLEND_ZERO, GR_BLEND_ZERO );
          grEnable(GR_AA_ORDERED);
          grVertexLayout(GR_PARAM_A,
                         GR_VERTEX_A_OFFSET << 2,
                         GR_PARAM_ENABLE);
          tlConOutput( "ANTIALIASED LINES \r" );
          break;
        }

        for( i = 0; i < 100; i+=2 ) {
            vtxA.x = tlScaleX( prjVerts[i].x );
            vtxA.y = tlScaleY( prjVerts[i].y );
            vtxA.oow = 1.0f / prjVerts[i].w;
            vtxA.a = 255.0f;
            vtxB.x = tlScaleX( prjVerts[i+1].x );
            vtxB.y = tlScaleY( prjVerts[i+1].y );
            vtxB.oow = 1.0f / prjVerts[i+1].w;
            vtxB.a = 255.0f;
            grDrawLine( &vtxA, &vtxB );
        }

        tlConRender();
        grBufferSwap( 1 );
        grFinish();
        
        while( tlKbHit() ) {
            switch( tlGetCH() ) {
            case 'a':
            case 'A':
                mode++;
                mode%=2;
                break;
            default:
                frames = 0;
                break;
            }
        }
    }
    
    grGlideShutdown();
    free(fogtable);
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




