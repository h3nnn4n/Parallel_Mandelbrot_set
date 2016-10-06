#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#include "types.h"
#include "mandel.h"
#include "image_utils.h"

#ifndef _SCHELL_
#define _SCHELL_ static
#endif

int main(int argc, char *argv[]) {
    int     block_size;
    int     ix, iy;
    int     *escapetime;
    _config config;
    _color  *bitmap;

    config.screenx  = 1920;
    config.screeny  = 1080;
    config.bailout  = 5000;
    config.er       =  2;

    /*config.minx     = -2.5;*/
    /*config.maxx     =  1.5;*/
    /*config.miny     = -2.0;*/
    /*config.maxy     =  2.0;*/

    config.minx     = -0.7436431355 - 0.000014628;
    config.maxx     = -0.7436431355 + 0.000014628;
    config.miny     =  0.131825963  - 0.000014628;
    config.maxy     =  0.131825963  + 0.000014628;

    block_size      =  100;

    if ( argc > 1 ) {
        omp_set_num_threads(atoi(argv[1]));
    }

    /*printf("%f \t %f\n%f\t %f\n", config.minx, config.maxx, config.miny, config.maxy);*/

    escapetime = ( int   * ) malloc ( sizeof ( int    ) * config.screenx * config.screeny );
    bitmap     = ( _color* ) malloc ( sizeof ( _color ) * config.screenx * config.screeny );

/*#pragma omp parallel for private(ix, iy, config) shared(escapetime)*/
#pragma omp parallel for private(ix) schedule(_SCHELL_)
    for ( iy = 0; iy < config.screeny; iy += block_size ) {
        for ( ix = 0; ix < config.screenx; ix += block_size ) {
            do_block(ix, ix+block_size-1, iy, iy+block_size-1, config, escapetime);
        }
        /*fprintf(stderr," -- %.2f%%\n",(iy/(double)config.screeny)*100.0);*/
    }

    for ( iy = 0; iy < config.screeny; iy++ ) {
        for ( ix = 0; ix < config.screenx; ix++ ) {
            bitmap[iy * config.screenx + ix].r = escapetime[iy * config.screenx + ix];
            bitmap[iy * config.screenx + ix].g = escapetime[iy * config.screenx + ix];
            bitmap[iy * config.screenx + ix].b = escapetime[iy * config.screenx + ix];
        }
    }

    /*fprintf(stderr," -- %.2f%%\n",100.0);*/
    /*fprintf(stderr," <---- DONE ---->\n");*/
    /*fprintf(stderr," Writing to disk!\n");*/

    save_png_to_file(bitmap, config.screenx, config.screeny, "mandel.png");

    free(escapetime);

    /*fprintf(stderr," -- Bye\n");*/

    return EXIT_SUCCESS;
}
