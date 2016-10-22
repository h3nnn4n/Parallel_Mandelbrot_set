#include "types.h"
#include "mandel.h"
#include "mandel_processor.h"

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
