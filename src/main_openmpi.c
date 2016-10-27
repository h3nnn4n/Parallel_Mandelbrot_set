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

    double  start_time, end_time;

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

    if ( argc == 2 ) {
        block_size = atoi(argv[1]);
    } else {
        block_size = 40;
    }

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    escapetime = ( int   * ) malloc ( sizeof ( int    ) * config.screenx * config.screeny );
    bitmap     = ( _color* ) malloc ( sizeof ( _color ) * config.screenx * config.screeny );

    if ( rank == 0 ) {
        int counter = size-1;
        int data_size = sizeof(int   ) * 4 // Block coordinates
                      + sizeof(int   )     // Suicide flag;
                      + sizeof(double) * 5 // Image complex coordinates + ER
                      + sizeof(int   ) * 3 // Screen size (pixels) + bailout
                      + sizeof(int   ) * 0;// Gambi
        void *outbuf =        malloc ( data_size                                               );
        int  *block  = (int*) malloc ( sizeof(int) * 4 + sizeof(int) * block_size * block_size );

        start_time = MPI_Wtime();
        for ( iy = 0; iy < config.screeny; iy += block_size ) {
            for ( ix = 0; ix < config.screenx; ix += block_size ) {
                int position = 0;
                int tmp;

                tmp = 1;
                MPI_Pack(&tmp           , 1, MPI_INT   , outbuf, data_size, &position, MPI_COMM_WORLD);

                tmp = ix;
                MPI_Pack(&tmp           , 1, MPI_INT   , outbuf, data_size, &position, MPI_COMM_WORLD);

                tmp = ix+block_size;
                MPI_Pack(&tmp           , 1, MPI_INT   , outbuf, data_size, &position, MPI_COMM_WORLD);

                tmp = iy;
                MPI_Pack(&tmp           , 1, MPI_INT   , outbuf, data_size, &position, MPI_COMM_WORLD);

                tmp = iy+block_size;
                MPI_Pack(&tmp           , 1, MPI_INT   , outbuf, data_size, &position, MPI_COMM_WORLD);

                MPI_Pack(&config.minx   , 1, MPI_DOUBLE, outbuf, data_size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.maxx   , 1, MPI_DOUBLE, outbuf, data_size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.miny   , 1, MPI_DOUBLE, outbuf, data_size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.maxy   , 1, MPI_DOUBLE, outbuf, data_size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.er     , 1, MPI_DOUBLE, outbuf, data_size, &position, MPI_COMM_WORLD);

                MPI_Pack(&config.bailout, 1, MPI_INT   , outbuf, data_size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.screenx, 1, MPI_INT   , outbuf, data_size, &position, MPI_COMM_WORLD);
                MPI_Pack(&config.screeny, 1, MPI_INT   , outbuf, data_size, &position, MPI_COMM_WORLD);

                if ( counter > 0 ) {
                    MPI_Send(outbuf, position, MPI_PACKED, counter, 0, MPI_COMM_WORLD);
                    counter--;
                } else {
                    MPI_Recv(block, 4 * block_size*block_size, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

                    int ii, jj;

                    int a1 = block[0];
                    int a2 = block[1];
                    int a3 = block[2];
                    int a4 = block[3];

                    int copier = 4;
                    for (jj = a3; jj < a4; jj++) {
                        for (ii = a1; ii < a2; ii++) {
                            if ( !((jj < config.screeny) && ( ii <= config.screenx )) ) {
                                copier++;
                                continue;
                            } else {
                                escapetime[ jj * config.screenx + ii] =  block[copier];
                                copier++;
                            }
                        }
                    }

                    MPI_Send(outbuf, position, MPI_PACKED, status.MPI_SOURCE, 0, MPI_COMM_WORLD         );
                }
            }
            fprintf(stderr," -- %.2f%%\n",((iy+block_size > config.screeny ? config.screeny : iy + block_size) /
                                           (double)config.screeny)*100.0);
        }

        int cc;
        for (cc = 1; cc < size; cc++) {
            int tmp = 0;
            int position = 0;
            MPI_Pack(&tmp, 1, MPI_INT, outbuf, data_size, &position, MPI_COMM_WORLD);
            MPI_Send(outbuf, position, MPI_PACKED, cc, 0, MPI_COMM_WORLD);
            MPI_Recv(block, 4 * block_size*block_size, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            int ii, jj;

            int a1 = block[0];
            int a2 = block[1];
            int a3 = block[2];
            int a4 = block[3];

            int copier = 4;
            for (jj = a3; jj < a4; jj++) {
                for (ii = a1; ii < a2; ii++) {
                    if ( !((jj < config.screeny) && ( ii <= config.screenx )) ) {
                        copier++;
                        continue;
                    } else {
                        escapetime[ jj * config.screenx + ii] =  block[copier];
                        copier++;
                    }
                }
            }
        }

        end_time = MPI_Wtime();

        /*MPI_Barrier(MPI_COMM_WORLD);*/

        printf("%d %f\n", size, end_time - start_time);

        for ( iy = 0; iy < config.screeny; iy++ ) {
            for ( ix = 0; ix < config.screenx; ix++ ) {
                bitmap[iy * config.screenx + ix].r = escapetime[iy * config.screenx + ix];
                bitmap[iy * config.screenx + ix].g = escapetime[iy * config.screenx + ix];
                bitmap[iy * config.screenx + ix].b = escapetime[iy * config.screenx + ix];
            }
        }

        save_png_to_file(bitmap, config.screenx, config.screeny, "mandel.png");
    } else {
        int data_size = sizeof(int   ) * 4 // Block coordinates
                      + sizeof(int   )     // Suicide flag;
                      + sizeof(double) * 5 // Image complex coordinates + ER
                      + sizeof(int   ) * 3 // Screen size (pixels) + bailout
                      + sizeof(int   ) * 0;// Gambi
        void *inbuf =        malloc ( data_size                                               );
        int  *block = (int*) malloc ( sizeof(int) * 4 + sizeof(int) * block_size * block_size );
        int flag    = 1;
        int counter = 0;
        while( flag ) {
            counter++;
            int position = 0;

            int a1, a2, a3, a4;
            _config config;

            MPI_Recv(inbuf, data_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD, &status);

            MPI_Unpack(inbuf, data_size, &position, &flag          , 1, MPI_INT   , MPI_COMM_WORLD);

            if ( !flag ) {
                continue;
            }

            MPI_Unpack(inbuf, data_size, &position, &a1            , 1, MPI_INT   , MPI_COMM_WORLD);
            MPI_Unpack(inbuf, data_size, &position, &a2            , 1, MPI_INT   , MPI_COMM_WORLD);
            MPI_Unpack(inbuf, data_size, &position, &a3            , 1, MPI_INT   , MPI_COMM_WORLD);
            MPI_Unpack(inbuf, data_size, &position, &a4            , 1, MPI_INT   , MPI_COMM_WORLD);

            MPI_Unpack(inbuf, data_size, &position, &config.minx   , 1, MPI_DOUBLE, MPI_COMM_WORLD);
            MPI_Unpack(inbuf, data_size, &position, &config.maxx   , 1, MPI_DOUBLE, MPI_COMM_WORLD);
            MPI_Unpack(inbuf, data_size, &position, &config.miny   , 1, MPI_DOUBLE, MPI_COMM_WORLD);
            MPI_Unpack(inbuf, data_size, &position, &config.maxy   , 1, MPI_DOUBLE, MPI_COMM_WORLD);
            MPI_Unpack(inbuf, data_size, &position, &config.er     , 1, MPI_DOUBLE, MPI_COMM_WORLD);

            MPI_Unpack(inbuf, data_size, &position, &config.bailout, 1, MPI_INT   , MPI_COMM_WORLD);
            MPI_Unpack(inbuf, data_size, &position, &config.screenx, 1, MPI_INT   , MPI_COMM_WORLD);
            MPI_Unpack(inbuf, data_size, &position, &config.screeny, 1, MPI_INT   , MPI_COMM_WORLD);

            do_block(a1, a2, a3, a4, config, escapetime);

            block[0] = a1;
            block[1] = a2;
            block[2] = a3;
            block[3] = a4;

            int ii, jj;
            int copier = 4;
            for (jj = a3; jj < a4; jj++) {
                for (ii = a1; ii < a2; ii++) {
                    if ( !((jj < config.screeny) && ( ii <= config.screenx )) ) {
                        copier++;
                        continue;
                    } else {
                        block[copier] = escapetime[ jj * config.screenx + ii ];
                        copier++;
                    }
                }
            }

            MPI_Send(block, 4 + block_size * block_size, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD );
        }
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
