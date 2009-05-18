#ifndef DEF_QUANTIZATION
#define DEF_QUANTIZATION

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Ya moyen qu'il y ait un soucis vis à vis de la taille apres
 * dequantification, (1 élément en moins)
 */

void *quantizate(float *x, size_t *n, const float factor, int *nbbits);
float *dequantizate(uint8_t *x, size_t *n, const float factor, int nbbits);

#endif
