#include <math.h>
#include "types.h"
#include "mandel.h"
#include "mandel_processor.h"

int process_point_aa(double cx, double cy, int er, int bailout, int aa, _config c) {
    /*return process_point(cx, cy, er, bailout);*/
    double dx, dy;
    double acc = 0;
    int i, j;
    int w = 0;

    dx = ((c.maxx - c.minx) / c.screenx ) / (double)aa;
    dy = ((c.maxy - c.miny) / c.screeny ) / (double)aa;

    for ( i = -aa/2; i < aa/2; ++i) {
        for ( j = -aa/2; j < aa/2; ++j) {
            w++;
            acc += process_point(cx + i * dx, cy + j * dy, er, bailout);
        }
    }

    return (int) (acc / ( w ));
}

int process_point(double cx, double cy, int er, int bailout) {
    int    i;
    double zx , zxn , zy , zyn;
    double zx2, zxn2, zy2, zyn2;

    double dist = 1e4;

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

        if ( zx2 * zx2 * 2 + zy2 * zy2 * 1 < dist * dist ) {
            dist = sqrt( zx2 * zx2 * 2 + zy2 * zy2 * 2 );
            /*printf("%f\n", dist);*/
        }

        if ( zx2 == zx && zy2 == zy ) {
            return 0;
        }

        if ( zx * zx + zy * zy > er * er ) {
            /*if ( zx > 0 ) {*/
                /*return dist * 1000;*/
                /*return 255;*/
            /*}*/
            /*return dist * 2000;*/
            /*return 0;*/
            /*return 255;*/
            /*return dist;*/
            return i;
        }
    }

    return 0;
}
