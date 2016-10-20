#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <mpi.h>

#include "types.h"
#include "mandel.h"
#include "image_utils.h"

int main(int argc, char *argv[]) {
    int     block_size;
    int     ix, iy;
    int     *escapetime;
    int     rank, size;
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

    /*config.minx     = -0.7436431355 - 0.000014628;*/
    /*config.maxx     = -0.7436431355 + 0.000014628;*/
    /*config.miny     =  0.131825963  - 0.000014628;*/
    /*config.maxy     =  0.131825963  + 0.000014628;*/

    config.minx     = -0.743643887037151 - 0.000000000051299;
    config.maxx     = -0.743643887037151 + 0.000000000051299;
    config.miny     =  0.131825904205330 - 0.000000000051299;
    config.maxy     =  0.131825904205330 + 0.000000000051299;

    block_size      =  100;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /*printf("%f \t %f\n%f\t %f\n", config.minx, config.maxx, config.miny, config.maxy);*/

    escapetime = ( int   * ) malloc ( sizeof ( int    ) * config.screenx * config.screeny );
    bitmap     = ( _color* ) malloc ( sizeof ( _color ) * config.screenx * config.screeny );

    printf("%d is up, %d total\n", rank, size);

    if ( rank == 0 ) {
        int counter = size-1;
        int size = sizeof(int   ) * 4 // Block coordinates
                 + sizeof(double) * 5 // Image complex coordinates + ER
                 + sizeof(int   ) * 3 // Screen size (pixels) + bailout
                 + sizeof(int   )    ;// Suicide flag;
        void *outbuf = malloc ( size );
        for ( iy = 0; iy < config.screeny; iy += block_size ) {
            for ( ix = 0; ix < config.screenx; ix += block_size ) {
                int position = 0;
                int    tmp;

                printf("Packing ");

                tmp = ix;
                printf("%d ", tmp);
                MPI_Pack(&tmp           , 1, MPI_INT   , outbuf, size, &position, MPI_COMM_WORLD);

                tmp = ix+block_size-1;
                printf("%d ", tmp);
                MPI_Pack(&tmp           , 1, MPI_INT   , outbuf, size, &position, MPI_COMM_WORLD);

                tmp = iy;
                printf("%d ", tmp);
                MPI_Pack(&tmp           , 1, MPI_INT   , outbuf, size, &position, MPI_COMM_WORLD);

                tmp = iy+block_size-1;
                printf("%d ", tmp);
                MPI_Pack(&tmp           , 1, MPI_INT   , outbuf, size, &position, MPI_COMM_WORLD);
                printf("\n");

                MPI_Pack(&config.minx   , 1, MPI_DOUBLE, outbuf, size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.maxx   , 1, MPI_DOUBLE, outbuf, size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.miny   , 1, MPI_DOUBLE, outbuf, size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.maxy   , 1, MPI_DOUBLE, outbuf, size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.er     , 1, MPI_DOUBLE, outbuf, size, &position, MPI_COMM_WORLD);

                MPI_Pack(&config.bailout, 1, MPI_INT   , outbuf, size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.screenx, 1, MPI_INT   , outbuf, size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.screeny, 1, MPI_INT   , outbuf, size, &position, MPI_COMM_WORLD);

                tmp = -1;
                MPI_Pack(&tmp           , 1, MPI_INT   , outbuf, size, &position, MPI_COMM_WORLD);

                if ( counter > 0 ) {
                    MPI_Send(outbuf, position, MPI_PACKED, counter, 0, MPI_COMM_WORLD);
                    printf("%d sent to %d\n", rank, counter);
                    counter--;
                } else {
                    /*do_block(ix, ix+block_size-1, iy, iy+block_size-1, config, escapetime);*/
                    int d = -1;
                    MPI_Recv(&d    , 1, MPI_INT   , MPI_ANY_SOURCE   , 0, MPI_COMM_WORLD, &status);
                    printf("master got %f\n", d);
                    MPI_Send(outbuf, 1, MPI_PACKED, status.MPI_SOURCE, 0, MPI_COMM_WORLD         );
                    printf("> %d sent to %d\n", rank, status.MPI_SOURCE);
                }
            }
            /*fprintf(stderr," -- %.2f%%\n",(iy/(double)config.screeny)*100.0);*/
        }
    } else {
        int size = sizeof(int   ) * 4 // Block coordinates
                 + sizeof(double) * 5 // Image complex coordinates + ER
                 + sizeof(int   ) * 3 // Screen size (pixels) + bailout
                 + sizeof(int   )    ;// Suicide flag;
        void *inbuf = malloc ( size );
        int position = 0;

        int a1, a2, a3, a4, flag;
        _config config;

        printf("%d is waiting\n", rank);
        MPI_Recv(inbuf, size, MPI_PACKED, 0, 0, MPI_COMM_WORLD, &status);

        MPI_Unpack(inbuf, size, &position, &a1            , 1, MPI_INT   , MPI_COMM_WORLD);
        MPI_Unpack(inbuf, size, &position, &a2            , 1, MPI_INT   , MPI_COMM_WORLD);
        MPI_Unpack(inbuf, size, &position, &a3            , 1, MPI_INT   , MPI_COMM_WORLD);
        MPI_Unpack(inbuf, size, &position, &a4            , 1, MPI_INT   , MPI_COMM_WORLD);

        MPI_Unpack(inbuf, size, &position, &config.minx   , 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(inbuf, size, &position, &config.maxx   , 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(inbuf, size, &position, &config.miny   , 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(inbuf, size, &position, &config.maxy   , 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(inbuf, size, &position, &config.er     , 1, MPI_DOUBLE, MPI_COMM_WORLD);

        MPI_Unpack(inbuf, size, &position, &config.bailout, 1, MPI_INT   , MPI_COMM_WORLD);
        MPI_Unpack(inbuf, size, &position, &config.screenx, 1, MPI_INT   , MPI_COMM_WORLD);
        MPI_Unpack(inbuf, size, &position, &config.screeny, 1, MPI_INT   , MPI_COMM_WORLD);

        MPI_Unpack(inbuf, size, &position, &flag          , 1, MPI_INT   , MPI_COMM_WORLD);

        printf("%d got %d %d %d %d %d\n", rank, a1, a2, a3, a4, flag);
        /*do_block(a1, a2, a3, a4, config, escapetime);*/

        int d = 0;
        MPI_Send(&d, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD );
    }


    for ( iy = 0; iy < config.screeny; iy++ ) {
        for ( ix = 0; ix < config.screenx; ix++ ) {
            bitmap[iy * config.screenx + ix].r = escapetime[iy * config.screenx + ix];
            bitmap[iy * config.screenx + ix].g = escapetime[iy * config.screenx + ix];
            bitmap[iy * config.screenx + ix].b = escapetime[iy * config.screenx + ix];
        }
    }

    MPI_Finalize();

    /*fprintf(stderr," -- %.2f%%\n",100.0);*/
    /*fprintf(stderr," <---- DONE ---->\n");*/
    /*fprintf(stderr," Writing to disk!\n");*/

    save_png_to_file(bitmap, config.screenx, config.screeny, "mandel.png");

    free(escapetime);

    /*fprintf(stderr," -- Bye\n");*/

    return EXIT_SUCCESS;
}
