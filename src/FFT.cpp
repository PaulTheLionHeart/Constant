/*
 * Xavier Gourdon : Sept. 99 (xavier.gourdon.free.fr)
 * 
 * FFT.c : Very basic FFT file.
 *         The FFT encoded in this file is short and very basic.
 *         It is just here as a teaching file to have a first 
 *         understanding of the FFT technique.
 *         
 * A lot of optimizations could be made to save a factor 2 or 4 for time
 * and space.
 *  - Use a 4-Step (or more) FFT to avoid data cache misses.
 *  - Use an hermitian FFT to take into account the hermitian property of
 *    the FFT of a real array.
 *  - Use a quad FFT (recursion N/4->N instead of N/2->N) to save 10 or
 *    15% of the time.
 *
 *  Informations can be found on 
 *    http://xavier.gourdon.free.fr/Constants/constants.html
 */
#include "FFT.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535897932384626

typedef struct ComplexTag {
  Real R,I;
} Complex;

long FFTLengthMax;
Complex * OmegaFFT;
Complex * ArrayFFT0, * ArrayFFT1;
Complex * ComplexCoef; 
double FFTSquareWorstError;
long AllocatedMemory;

void InitializeFFT(long MaxLength)
{
  long i;
  Real Step;

  FFTLengthMax = MaxLength;
  OmegaFFT = (Complex *) malloc(MaxLength/2*sizeof(Complex));
  ArrayFFT0 = (Complex *) malloc(MaxLength*sizeof(Complex));
  ArrayFFT1 = (Complex *) malloc(MaxLength*sizeof(Complex));
  ComplexCoef = (Complex *) malloc(MaxLength*sizeof(Complex));
  Step = 2.*PI/(double) MaxLength;
  for (i=0; 2*i<MaxLength; i++) {
    OmegaFFT[i].R = cos(Step*(double)i);
    OmegaFFT[i].I = sin(Step*(double)i);
  }
  FFTSquareWorstError=0.;
  AllocatedMemory = 7*MaxLength*sizeof(Complex)/2;
}

void RecursiveFFT(Complex * Coef, Complex * FFT, long Length, long Step, 
		  long Sign)
{
  long i, OmegaStep;
  Complex * FFT0, * FFT1, * Omega;
  Real tmpR, tmpI;

  if (Length==2) {
    FFT[0].R = Coef[0].R + Coef[Step].R;
    FFT[0].I = Coef[0].I + Coef[Step].I;
    FFT[1].R = Coef[0].R - Coef[Step].R;
    FFT[1].I = Coef[0].I - Coef[Step].I;
    return;
  }

  FFT0 = FFT;
  RecursiveFFT(Coef     ,FFT0,Length/2,Step*2,Sign);
  FFT1 = FFT+Length/2;
  RecursiveFFT(Coef+Step,FFT1,Length/2,Step*2,Sign);

  Omega = OmegaFFT;
  OmegaStep = FFTLengthMax/Length;
  for (i=0; 2*i<Length; i++, Omega += OmegaStep) {
    /* Recursion formula for FFT :
       FFT[i]          <-  FFT0[i] + Omega*FFT1[i]
       FFT[i+Length/2] <-  FFT0[i] - Omega*FFT1[i],
       Omega = exp(2*I*PI*i/Length) */
    tmpR = Omega[0].R*FFT1[i].R-Sign*Omega[0].I*FFT1[i].I;
    tmpI = Omega[0].R*FFT1[i].I+Sign*Omega[0].I*FFT1[i].R;
    FFT1[i].R = FFT0[i].R - tmpR;
    FFT1[i].I = FFT0[i].I - tmpI;
    FFT0[i].R = FFT0[i].R + tmpR;
    FFT0[i].I = FFT0[i].I + tmpI;
  }
}

/* Compute the complex Fourier Transform of Coef into FFT */
void FFT(Real * Coef, long Length, Complex * FFT, long NFFT)
{
  long i;
  /* Transform array of real coefficient into array of complex */
  for (i=0; i<Length; i++) {
    ComplexCoef[i].R = Coef[i];
    ComplexCoef[i].I = 0.;
  }
  for (; i<NFFT; i++)
    ComplexCoef[i].R = ComplexCoef[i].I = 0.;

  RecursiveFFT(ComplexCoef,FFT,NFFT,1,1);
}

/* Compute the inverse Fourier Transform of FFT into Coef */
void InverseFFT(Complex * FFT, long NFFT, Real * Coef, long Length)
{
  long i;
  Real invNFFT = 1./(Real) NFFT, tmp;

  RecursiveFFT(FFT, ComplexCoef, NFFT, 1, -1);
  for (i=0; i<Length; i++) {
    /* Closest integer to ComplexCoef[i].R/NFFT */
    tmp = invNFFT*ComplexCoef[i].R;
    Coef[i] = floor(0.5+tmp);
    if ((tmp-Coef[i])*(tmp-Coef[i])>FFTSquareWorstError)
      FFTSquareWorstError = (tmp-Coef[i])*(tmp-Coef[i]);
  }
}

void Convolution(Complex * A, Complex * B, long NFFT, Complex * C)
{
  long i;
  Real tmpR, tmpI;

  for (i=0; i<NFFT; i++) {
   tmpR = A[i].R*B[i].R-A[i].I*B[i].I;
    tmpI = A[i].R*B[i].I+A[i].I*B[i].R;
    C[i].R = tmpR;
    C[i].I = tmpI;
  }
}

void MulWithFFT(Real * ACoef, long ASize,
                Real * BCoef, long BSize,
                Real * CCoef)
{
  long NFFT = 2;

  while (NFFT<ASize+BSize)
    NFFT *= 2;

  if (NFFT>FFTLengthMax) {
    printf("Error, FFT Size is too big in MulWithFFT\n");
    return;
  }
  FFT(ACoef, ASize, ArrayFFT0, NFFT);
  FFT(BCoef, BSize, ArrayFFT1, NFFT);
  Convolution(ArrayFFT0,ArrayFFT1,NFFT,ArrayFFT0);
  InverseFFT(ArrayFFT0,NFFT,CCoef, ASize+BSize-1);
}
