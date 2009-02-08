#include <stdlib.h>
#include <stdint.h>

#include "half.h"

int main()
{
    float f = -1;
    t_half h = float_to_half(f);

    printf("%hi\n", h);

    return 0;
}
