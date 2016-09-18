#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "types.h"

int process_point(double cx, double cy, int er, int bailout) {
    int    i;
    double zx , zxn , zy , zyn;
    double zx2, zxn2, zy2, zyn2;

    zx  = 0.0;
    zy  = 0.0;
    zx2 = 0.0;
    zy2 = 0.0;

    for ( i = 0; i < bailout; i++ ){
        zxn = zx * zx - zy * zy + cx;
        zyn = 2.0 * zx * zy + cy;
        zx  = zxn; zy  = zyn;

        zxn = zx * zx - zy * zy + cx;
        zyn = 2.0 * zx * zy + cy;
        zx  = zxn; zy  = zyn;

        zxn2 = zx2 * zx2 - zy2 * zy2 + cx;
        zyn2 = 2.0 * zx2 * zy2 + cy;
        zx2  = zxn2; zy2  = zyn2;

        if ( zx2 == zx && zy2 == zy )
            return 0;

        if ( zx * zx + zy * zy > er * er )
            return i;
    }

    return 0;
}

void fill_block(int ix_min, int ix_max, int iy_min, int iy_max, _config c, int *img, int aa) {
    int y = iy_min;
    for (; ix_min <= ix_max; ++ix_min) {
        for (iy_min = y; iy_min <= iy_max; ++iy_min) {
            img[ iy_min * c.screenx + ix_min ] = aa;
        }
    }
}

void finish_block(int ix_min, int ix_max, int iy_min, int iy_max, _config c, int *img) {
    int iy, ix;
    double cx, cy;

    for(iy = iy_min; iy < iy_max; iy++ ){
        cy = c.miny + iy*(c.maxy - c.miny) / c.screeny;
        for ( ix = ix_min; ix < ix_max; ix++ ) {
            cx = c.minx + ix * (c.maxx - c.minx) / c.screenx;
            img[iy * c.screenx + ix] = process_point(cx, cy, c.er, c.bailout);
        }
    }
}

void do_block(int ix_min, int ix_max, int iy_min, int iy_max, _config c, int *img) {
    if ( ix_max > c.screenx ) ix_max = c.screenx - 1;
    if ( iy_max > c.screeny ) iy_max = c.screeny - 1;

    int dx = (ix_max - ix_min ) / 2;
    int dy = (iy_max - iy_min ) / 2;

    int const eps = 6;

    double cx_min, cx_max;
    double cy_min, cy_max;

    cx_min = c.minx + ix_min * (c.maxx - c.minx) / c.screenx;
    cx_max = c.minx + ix_max * (c.maxx - c.minx) / c.screenx;

    cy_min = c.miny + iy_min * (c.maxy - c.miny) / c.screeny;
    cy_max = c.miny + iy_max * (c.maxy - c.miny) / c.screeny;

    int aa = process_point(cx_min, cy_min, c.er, c.bailout);
    int bb = process_point(cx_max, cy_max, c.er, c.bailout);
    int ab = process_point(cx_min, cy_max, c.er, c.bailout);
    int ba = process_point(cx_max, cy_min, c.er, c.bailout);

    if ( aa == bb  && aa == ab && aa == ba ) {
        fill_block(ix_min, ix_max, iy_min, iy_max, c, img, aa);
    } else {
        if ( dx < eps && dy < eps ) {
            finish_block(ix_min , ix_max     , iy_min     , iy_max     , c, img);
        } else if ( dy < eps ) {
            do_block(ix_min     , ix_max - dx, iy_min     , iy_max     , c, img);
            do_block(ix_min + dx, ix_max     , iy_min     , iy_max     , c, img);
        } else if ( dx < eps ) {
            do_block(ix_min     , ix_max     , iy_min     , iy_max - dy, c, img);
            do_block(ix_min     , ix_max     , iy_min + dy, iy_max     , c, img);
        } else {
            do_block(ix_min     , ix_max - dx, iy_min     , iy_max - dy, c, img);
            do_block(ix_min + dx, ix_max     , iy_min + dy, iy_max     , c, img);
            do_block(ix_min + dx, ix_max     , iy_min     , iy_max - dy, c, img);
            do_block(ix_min     , ix_max - dx, iy_min + dy, iy_max     , c, img);
        }
    }
}

int main(){
    int     block_size;
    int     ix, iy,
            i, j;
    _config config;

    FILE *img = fopen("mandel.ppm","wt");
    _color col;
    int *escapetime;

    config.screenx  = 1920;
    config.screeny  = 1080;
    config.bailout  = 5000;
    config.er       =  2;
    config.minx     = -2.5;
    config.maxx     =  1.5;
    config.miny     = -2.0;
    config.maxy     =  2.0;
    block_size      =  200;

    printf("%f \t %f\n%f\t %f\n", config.minx, config.maxx, config.miny, config.maxy);

    escapetime  =  (int*)malloc(sizeof(int)*config.screenx*config.screeny);
    fprintf(img, "P3\n%d %d\n255\n", config.screenx, config.screeny);

    /*finish_block(0, config.screenx, 0, config.screeny, config, escapetime);*/

    for ( iy = 0; iy < config.screeny; iy += block_size ) {
        for ( ix = 0; ix < config.screenx; ix += block_size ) {
            do_block(ix, ix+block_size-1, iy, iy+block_size-1, config, escapetime);
        }
        fprintf(stderr," -- %.2f%%\n",(iy/(double)config.screeny)*100.0);
    }

    fprintf(stderr," -- %.2f%%\n",100.0);
    fprintf(stderr," <---- DONE ---->\n");
    fprintf(stderr," Writing to disk!\n");

    for(i = 0; i < config.screeny; i++){
        for(j = 0; j < config.screenx; j++){
            col.r = escapetime[i*config.screenx+j] % 256;
            fprintf(img,"%d %d %d ",
                    (int)col.r * 1,
                    (int)col.r * 1,
                    (int)col.r * 1);
        }
        fputc('\n',img);
    }

    fclose(img);
    free(escapetime);

    fprintf(stderr," -- Bye\n");

    return EXIT_SUCCESS;
}
