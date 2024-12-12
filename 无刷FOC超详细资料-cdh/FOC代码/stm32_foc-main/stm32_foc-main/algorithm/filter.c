#include "filter.h"

void low_pass_filter(const float last, float *now, float alpha)
{
    *now = alpha * last + (1 - alpha) * (*now);
}