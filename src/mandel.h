#ifndef MANDEL_H
#define MANDEL_H

#include "types.h"

void fill_block(int ix_min, int ix_max, int iy_min, int iy_max, _config c, int *img, int aa);
void finish_block(int ix_min, int ix_max, int iy_min, int iy_max, _config c, int *img);
void do_block(int ix_min, int ix_max, int iy_min, int iy_max, _config c, int *img);
int check(int ix_min, int ix_max, int iy_min, int iy_max, _config c, int w, int color);

#endif /* MANDEL_H */
