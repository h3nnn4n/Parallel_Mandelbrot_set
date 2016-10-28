#ifndef TYPES_H
#define TYPES_H

typedef struct {
        double r, g, b;
} _color;

typedef struct {
    int    aa,
           bailout,
           screenx,
           screeny;
    double minx,
           maxx,
           miny,
           maxy,
           er;
} _config;


#endif /*  */
