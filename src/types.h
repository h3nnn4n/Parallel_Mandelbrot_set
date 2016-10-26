#ifndef TYPES_H
#define TYPES_H

#include <gmp.h>

typedef struct {
        double r, g, b;
} _color;

typedef struct {
    int    bailout,
           screenx,
           screeny;
    mpf_t  minx,
           maxx,
           miny,
           maxy;
    double er;
} _config;


#endif /*  */
