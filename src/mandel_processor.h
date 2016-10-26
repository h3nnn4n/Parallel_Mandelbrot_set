#ifndef MANDEL_PROCESSOR_H
#define MANDEL_PROCESSOR_H

#include "types.h"
#include "mandel.h"

#include <gmp.h>

int process_point(mpf_t cx,mpf_t cy, int er, int bailout);

#endif /* MANDEL_PROCESSOR_H */
