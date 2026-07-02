/*
 * Xavier Gourdon : Sept. 99 (xavier.gourdon.free.fr)
 * 
 * e.c : Basic file for computation of e=2.7181... with the binary
 *       splitting method.
 *       It relies on the files FFT.c and BigInt.c to handle
 *       Large Integers.
 *         
 * Several optimizations can be made :
 *  - Use a specialized routine to handle binary splitting when the
 *    indexes are close.
 *  ...
 *
 *  Informations can be found on 
 *    http://xavier.gourdon.free.fr/Constants/constants.html
 */

#include "FFT.h"
#include "BigInt.h"

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <windows.h>

BigInt tmpBigInt; /* Temporary BigInt used in the binary splitting */

/*
 * Compute the numerator P and denominator Q of 
 * P/Q = 1/(N0+1) + 1/(N0+1)/(N0+2) + ... + 1/(N0+1)/.../N1
 */
void BinarySplittingE(long N0, long N1, BigInt * P, BigInt * Q)
{
  BigInt PP, QQ;
  long NMid;

  if (N1-N0==1) {
    P->Size = Q->Size = 1;
    P->Coef[0] = 1.;
    Q->Coef[0] = (double) N1;
    UpdateBigInt(P);
    UpdateBigInt(Q);
    return;
  }
  NMid = (N0+N1)/2;
  BinarySplittingE(N0,NMid,P,Q);
  /* To save memory, take the non used coefficients of P and Q
     for coefficient of the BigInt used in the second splitting part */
  PP.Coef = P->Coef + P->Size;
  PP.SizeMax = P->SizeMax-P->Size;
  QQ.Coef = Q->Coef + Q->Size;
  QQ.SizeMax = Q->SizeMax-Q->Size;

  BinarySplittingE(NMid,N1,&PP,&QQ);
  MulBigInt(P,&QQ,&tmpBigInt);
  AddBigInt(&tmpBigInt,&PP,P);

  MulBigInt(Q,&QQ,Q);
}

/*
 * Returns as a BigInt the constant E with NbDec decimal digits
 */
BigInt ECompute(HWND hwnd, long NbDec)
    {
    BigInt P, Q, tmp;
    long i, MaxSize, MaxFFTSize, SeriesSize;
    double logFactorial, logMax;
    char s[80];

  /* MaxSize should be more than NbDec/NBDEC_BASE (see BinarySplitting) */
    MaxSize = NbDec/NBDEC_BASE + 10 + (long) (2.*log((double)NbDec)/log(2.));
    InitializeBigInt(&P,MaxSize);
    InitializeBigInt(&Q,MaxSize);

    MaxFFTSize = 2;
    while (MaxFFTSize < MaxSize)
	MaxFFTSize *= 2;
    MaxFFTSize *= 2;
    InitializeFFT(MaxFFTSize);
  /* Temporary BigInts are needed */
    InitializeBigInt(&tmpBigInt,MaxFFTSize);
    InitializeBigInt(&tmp,MaxFFTSize);

    sprintf(s, "Total Allocated memory = %ld K",AllocatedMemory/1024);
    SetWindowText (hwnd, s);

  // Compute the number of term SeriesSize of the series required.
  // SeriesSize is such that log(SeriesSize !) > NbDec*log(10.)
    SeriesSize=1;
    logFactorial=0.;
    logMax = (double)NbDec * log(10.);
    while (logFactorial<logMax) 
	{
	logFactorial += log((double) SeriesSize);
	SeriesSize++;
	}

    sprintf(s, "Starting series computation");
    SetWindowText (hwnd, s);
  // Compute the numerator P and the denominator Q of sum_{i=0}^{SeriesSize} 1/i!
    BinarySplittingE(0,SeriesSize,&P,&Q);
    AddBigInt(&P,&Q,&P);

    printf(s, "Starting final division");
    SetWindowText (hwnd, s);
  // Compute the inverse of Q in tmpBigInt
    Inverse(&Q,&tmpBigInt,&tmp);
  // Comute P/Q in tmpBigInt
    MulBigInt(&P,&tmpBigInt,&tmpBigInt);

  // Put the number of required digits in P 
    P.Size = 1+NbDec/NBDEC_BASE;
    for (i=1; i<=P.Size; i++)
	P.Coef[P.Size-i] = tmpBigInt.Coef[tmpBigInt.Size-i];

    FreeBigInt(&Q);
    FreeBigInt(&tmpBigInt);

    return P;
    }


void eFastMain(HWND hwnd, char *output, char *name, char *Time)
    {
    double StartTime;
    BigInt E;

    SetWindowText (hwnd, "Initialising");
    StartTime = (double) clock();
    E = ECompute(hwnd, NbDigits);
    sprintf_s (name, 100, "Constant e = ");
    PrintBigInt(&E, output);
    sprintf_s (Time, 100, "Computation time is : %9.2f seconds\n",  (float)(clock() - StartTime)/(float)CLOCKS_PER_SEC ); 
    SetWindowText (hwnd, "Paul's Constant Generator");
    }