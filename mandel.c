#include "types.h"
#include "mandel.h"

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

int check(int ix_min, int ix_max, int iy_min, int iy_max, _config c, int w, int color){
    int iy, ix;
    double cx, cy;

    for(iy = iy_min; iy < iy_max; iy += w ){
        cy = c.miny + iy*(c.maxy - c.miny) / c.screeny;

        if ( process_point(c.minx + (ix_max) * (c.maxx - c.minx) / c.screenx, cy, c.er, c.bailout) != color )
            return 0;

        if ( process_point(c.minx + ( 0    ) * (c.maxx - c.minx) / c.screenx, cy, c.er, c.bailout) != color )
            return 0;
    }

    for ( ix = ix_min; ix < ix_max; ix += w ) {
        cx = c.minx + (ix_max) * (c.maxx - c.minx) / c.screenx;

        if ( process_point(cx, c.miny + (iy_max) * (c.maxy - c.miny) / c.screeny, c.er, c.bailout) != color )
            return 0;

        if ( process_point(cx, c.miny + ( 0    ) * (c.maxy - c.miny) / c.screeny, c.er, c.bailout) != color )
            return 0;
    }

    return 1;
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

    /*if ( aa == bb  && aa == ab && aa == ba ) {*/
    if ( aa == bb  && aa == ab && aa == ba && check(ix_min, ix_max, iy_min, iy_max, c, 10, aa) ) {
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
