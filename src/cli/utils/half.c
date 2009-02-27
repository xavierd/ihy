#include "half.h"

t_half float_to_half(t_float f)
{
    t_half h;

    h.negative = f.negative;
    h.exponent = f.exponent;
    h.mantissa = f.mantissa >> 13;

    return h;
}

t_float half_to_float(t_half h)
{
    t_float f;

    f.negative = h.negative;
    f.exponent = h.exponent;
    f.mantissa = h.mantissa << 13;

    return f;
}
