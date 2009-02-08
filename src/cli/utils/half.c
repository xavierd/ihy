#include "half.h"

t_half float_to_half(float f)
{
    t_half h;
    uint32_t f2;

    memcpy(&f2, &f, sizeof(float));
    printf("%i", f2);
    h = (f2 >> 31) << 15;

    return h;
}
