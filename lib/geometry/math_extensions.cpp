#include "math_extensions.h"

#include <math.h>

unsigned long clamp_and_floor(double value, unsigned long low,
                              unsigned long high) {
    if (value < (double)low) return low;
    if (value >= (double)high) return high - 1;
    return (unsigned long)floor(value);
}