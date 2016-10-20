#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

#include "types.h"
#include "mandel.h"
#include "image_utils.h"

typedef struct {
    _config config;
    int id;
    int *esctime;
    int block_size;
    int nt;
} _data;

void* feeder(void *data) {
    int     ix, iy;

    _config config  = ( ( _data* ) data )->config     ;
    int block_size  = ( ( _data* ) data )->block_size ;
    int id          = ( ( _data* ) data )->id         ;
    int nt          = ( ( _data* ) data )->nt         ;
    int *escapetime = ( ( _data* ) data )->esctime    ;

    for ( iy = id*block_size; iy < config.screeny; iy += block_size*nt ) {
        for ( ix = 0; ix < config.screenx; ix += block_size ) {
            do_block(ix, ix+block_size, iy, iy+block_size, config, escapetime);
        }
        /*fprintf(stderr," -- %.2f%%\n",(iy/(double)config.screeny)*100.0);*/
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int       block_size;
    int       ix, iy, i;
    int       *escapetime;
    int       nt;
    _config   config;
    _color    *bitmap;
    _data     *data;
    pthread_t *ts;

    config.screenx  = 1920;
    config.screeny  = 1080;
    config.bailout  = 5000;
    config.er       = 2;

    if ( argc > 1 ) {
        nt = atoi(argv[1]);
    } else {
        nt = 1;
    }

    /*config.minx     = -2.5;*/
    /*config.maxx     =  1.5;*/
    /*config.miny     = -2.0;*/
    /*config.maxy     =  2.0;*/

    config.minx     = -0.7436431355 - 0.000014628;
    config.maxx     = -0.7436431355 + 0.000014628;
    config.miny     =  0.131825963  - 0.000014628;
    config.maxy     =  0.131825963  + 0.000014628;

    block_size      =  100;

    /*printf("%f \t %f\n%f\t %f\n", config.minx, config.maxx, config.miny, config.maxy);*/

    ts         = ( pthread_t* ) malloc ( sizeof (pthread_t) * nt                              );
    data       = ( _data*     ) malloc ( sizeof ( _data   ) * nt                              );
    escapetime = ( int   *    ) malloc ( sizeof ( int     ) * config.screenx * config.screeny );
    bitmap     = ( _color*    ) malloc ( sizeof ( _color  ) * config.screenx * config.screeny );

    for ( i = 0; i < nt; ++i) {
        data[i].config     = config;
        data[i].id         = i;
        data[i].esctime    = escapetime;
        data[i].block_size = block_size;
        data[i].nt         = nt;
        pthread_create(&ts[i], NULL, feeder, (void*)&data[i]);
    }

    for ( i = 0; i < nt; ++i) {
        pthread_join(ts[i], NULL);
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
