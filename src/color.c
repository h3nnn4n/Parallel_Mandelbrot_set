#include "color.h"

void populatePal(_color *pallete){
    int size, i, r, g, b;
    char limbo[256];

    FILE *pal = fopen("pals/sunrise.ppm", "rt");

    if ( pal == NULL ) {
        fprintf(stderr, "Could not open palete for reading\n");
        abort();
    }

    fgets(limbo, 255, pal);
    fgets(limbo, 255, pal);
    fgets(limbo, 255, pal);

    fscanf(pal, "%d\n", &size);

    if( size > 0 && pal != NULL ){
        for (i = 0; i < size; ++i) {
            fscanf(pal, "%d\n%d\n%d\n", &r, &g, &b);
            pallete[i].r = r;
            pallete[i].g = g;
            pallete[i].b = b;
        }
    } else {
        fprintf(stderr, "Size if %d and pal is %p\nSomething is wrong!\n", size, pal);
        abort();
    }

    fclose(pal);
}

_color getPalMem(double p, _color *pal){
    _color col;
    memcpy( &col, &pal[ (int)(p * 255.0) ], sizeof(_color) );
    /*return invert_color(col);*/
    return col;
}

_color invert_color(_color col){
    col.r = 255 - col.r;
    col.g = 255 - col.g;
    col.b = 255 - col.b;
    return col;
}

