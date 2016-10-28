#ifndef MANDEL_PROCESSOR_H
#define MANDEL_PROCESSOR_H

#include "types.h"
#include "mandel.h"

int process_point(double cx, double cy, int er, int bailout);
int process_point_aa(double cx, double cy, int er, int bailout, int aa, _config c);

#endif /* MANDEL_PROCESSOR_H */
