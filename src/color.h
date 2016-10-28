#ifndef COLOR_H
#define COLOR_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"

//typedef struct {
    //double r, g, b;
//} _color;

void   populatePal  ( _color *pallete        ) ;
_color getPalMem    ( double p , _color *pal ) ;
_color invert_color ( _color col             ) ;

#endif /*  */
