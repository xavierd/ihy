#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>
#include <string.h>
#include <math.h>

#include "codecs/wav.h"

int main(int argc, char **argv) {
    fftw_complex *rdata = NULL, *idata = NULL;
    fftw_plan rplan;
    int n;
    double *A;
    double cc;
    FILE *file;
    double correction;
    int i,m,k, v;
    char erg[35];
    double absval;
    wav_data *input;

    if (argc < 2)
	exit(0);
    input = create_wav();
    read_wav(argv[1], input);
    n = (input->DataBlocSize / (input->BitsPerSample / 8)) / input->NumChannels;
    correction = (double)input->SampleRate / (double)n;
    file = fopen("data.dat", "w");

    /* read data and fill "A"-array */
    A = malloc(n * sizeof(double));
    i = 0;
    for (v=0;v<n;v++) {
	k = input->Data[i];
	i++;
	if (input->BitsPerSample == 16) {
	    m = input->Data[i];
	    i++;
	    A[v] = 256*m + k;
	} else {
	    A[v] = k;
	}
	if (input->NumChannels > 1)
	    i += (input->BitsPerSample / 8);
    }

    /* prepare fft with fftw*/
    rdata = fftw_malloc(n * sizeof(fftw_complex));
    idata = fftw_malloc(n * sizeof(fftw_complex));
    /* create the fftw plan */
    rplan = fftw_plan_dft_1d(n, rdata, idata, FFTW_FORWARD, FFTW_ESTIMATE);

    /* we have no imaginary data, so clear idata */
    memset((void *)idata, 0, n * sizeof(fftw_complex));
    /* fill rdata with actual data */
    for (i = 0; i < n; i++)
	rdata[i] = A[i];
    /* make fft */
    fftw_execute(rplan);

    /* post-process FFT data: make absolute values, and calculate
       real frequency of each power line in the spectrum*/
    m = 0;
    for (i = 0; i < (n / 2); i++) {
	absval = sqrt(idata[i] * idata[i]) / n;
	cc = (double)m * correction;
	sprintf(erg, "%f %f\n", cc, absval);
	fputs(erg, file);
	m++;
    }

    /* housekeeping */
    fclose(file);
    fftw_destroy_plan(rplan);
    fftw_free(rdata);
    fftw_free(idata);
    return 1;
}

