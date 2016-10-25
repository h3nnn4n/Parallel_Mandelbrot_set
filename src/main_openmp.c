#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#include <gmp.h>

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
    int     max;
    _config config;
    _color  *bitmap;

    mpf_t   minx, miny, maxx, maxy;

    mpf_set_default_prec(1024);

    mpf_init(minx);mpf_init(miny);
    mpf_init(maxx);mpf_init(maxy);

    config.screenx  = 1920;
    config.screeny  = 1080;
    config.bailout  = 5000;
    config.er       =  2;

    /*config.minx     = -2.5;*/
    /*config.maxx     =  1.5;*/
    /*config.miny     = -2.0;*/
    /*config.maxy     =  2.0;*/

    /*config.minx     = -0.7436431355 - 0.000014628;*/
    /*config.maxx     = -0.7436431355 + 0.000014628;*/
    /*config.miny     =  0.131825963  - 0.000014628;*/
    /*config.maxy     =  0.131825963  + 0.000014628;*/

    /*config.minx     = -0.743643887037151 - 0.000000000051299;*/
    /*config.maxx     = -0.743643887037151 + 0.000000000051299;*/
    /*config.miny     =  0.131825904205330 - 0.000000000051299;*/
    /*config.maxy     =  0.131825904205330 + 0.000000000051299;*/

    mpf_set_d( config.minx, -0.743643135 - 0.00012068 / 64.0 );
    mpf_set_d( config.maxx, -0.743643135 + 0.00012068 / 64.0 );
    mpf_set_d( config.miny,  0.131825963 - 0.00012068 / 64.0 );
    mpf_set_d( config.maxy,  0.131825963 + 0.00012068 / 64.0 );

    block_size      =  32;
    max             =  0;

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
            do_block(ix, ix+block_size, iy, iy+block_size, config, escapetime);
        }
        fprintf(stderr," -- %.2f%%\n",(iy/(double)config.screeny)*100.0);
    }

    for ( iy = 0; iy < config.screeny; iy++ ) {
        for ( ix = 0; ix < config.screenx; ix++ ) {
            max = max > escapetime[iy * config.screenx + ix] ? max : escapetime[iy * config.screenx + ix];
        }
    }

    for ( iy = 0; iy < config.screeny; iy++ ) {
        for ( ix = 0; ix < config.screenx; ix++ ) {
            bitmap[iy * config.screenx + ix].r = (int) (( escapetime[iy * config.screenx + ix] / (double) max ) * 255.0 );
            bitmap[iy * config.screenx + ix].g = (int) (( escapetime[iy * config.screenx + ix] / (double) max ) * 255.0 );
            bitmap[iy * config.screenx + ix].b = (int) (( escapetime[iy * config.screenx + ix] / (double) max ) * 255.0 );
        }
    }

    fprintf(stderr," -- %.2f%%\n",100.0);
    fprintf(stderr," <---- DONE ---->\n");
    fprintf(stderr," Writing to disk!\n");

    save_png_to_file(bitmap, config.screenx, config.screeny, "mandel.png");

    free(escapetime);

    /*fprintf(stderr," -- Bye\n");*/

    return EXIT_SUCCESS;
}
