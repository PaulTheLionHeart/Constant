/*
 * Xavier Gourdon : Sept. 99 (xavier.gourdon.free.fr)
 * 
 * BigInt.c : Basic file for manipulation of Large Integers.
 *            It rely on FFT.c to multiply number with the FFT
 *            technique.
 *         
 * Several optimizations can be made :
 *  - Implementation of a classic multiplication for small BigInts.
 *  - Avoid using UpdateBigInt for the sum of BigInts.
 *  - Use a larger base for the internal data storage of BigInts to
 *    save space.
 *  ...
 *
 *  Informations can be found on 
 *    http://xavier.gourdon.free.fr/Constants/constants.html
 */

#include "BigInt.h"
#include "FFT.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void InitializeBigInt(BigInt * A, long MaxSize)
{
  A->Coef = (Real *) malloc(MaxSize*sizeof(Real));
  AllocatedMemory += MaxSize*sizeof(Real);

  if (A->Coef==0)
    printf("Not enough memory\n");
  A->Size = 0;
  A->SizeMax = MaxSize;
}

void FreeBigInt(BigInt * A)
{
  free(A->Coef);
}

void PrintBigInt(BigInt * A, char *buffer)
    {
    long i,j,Digit=0,Dec;
    double Pow, Coef;
    char	buf[80];

    sprintf_s (buffer, 80, "%.0lf.",A->Coef[A->Size-1]);
//  printf("%.0lf\n",A->Coef[A->Size-1]);

//    *buffer = '\0';
    for (i=A->Size-2; i>=0; i--) 
	{
	Pow = BASE*0.1;
	Coef = A->Coef[i];
	for (j=0; j<NBDEC_BASE; j++) 
	    {
	    Dec = (long) (Coef/Pow);
	    Coef -= Dec*Pow;
	    Pow *= 0.1;
//	    printf("%ld",Dec);
	    sprintf_s (buf, 80, "%ld",Dec);
	    strcat_s(buffer, NbDigits*2-1, buf);
	    Digit++;
//	    if (Digit%10==0) printf(" ");
//	    if (Digit%50==0) printf(": %ld\n",Digit);
	    }
	}
    }

/*
 * Update A to the normal form (0<= A.Coef[i] < BASE)
 */
void UpdateBigInt(BigInt * A)
{
  long i;
  Real carry=0., x;

  for (i=0; i<A->Size; i++) {
    x = A->Coef[i] + carry;
    carry = floor(x*invBASE);
    A->Coef[i] = x-carry*BASE;
  }
  if (carry!=0) {
    while (carry!=0.) {
      x = carry;
      carry = floor(x*invBASE);
      A->Coef[i] = x-carry*BASE;
      i++;
      A->Size=i;
    }
    if (A->Size > A->SizeMax) {
      printf("Error in UpdateBigInt, Size>SizeMax\n");
    }
  }
  else {
    while (i>0 && A->Coef[i-1]==0.) i--;
    A->Size=i;
  }
}

/*
 * Compute C = A + B
 */
void AddBigInt(BigInt * A, BigInt * B, BigInt * C)
{
  long i;

  if (A->Size<B->Size) {
    AddBigInt(B,A,C);
    return;
  }
  /* We now have B->Size<A->Size */
  for (i=0; i<B->Size; i++)
    C->Coef[i] = A->Coef[i] + B->Coef[i];
  for (; i<A->Size; i++) 
    C->Coef[i] = A->Coef[i];

  C->Size = A->Size;
  UpdateBigInt(C);
}

/*
 * Compute C = A*B
 */
void MulBigInt(BigInt * A, BigInt * B, BigInt * C)
{
  MulWithFFT(A->Coef,A->Size,B->Coef,B->Size,C->Coef);
  C->Size = A->Size+B->Size-1;
  UpdateBigInt(C);
}

/*
 * Compute the inverse of A in B
 */
void Inverse(BigInt * A, BigInt * B, BigInt * tmpBigInt)
{
  double x;
  long i, N,NN,Delta;
  int Twice=1, Sign;
  BigInt AA;

  /* Initialization */
  x = A->Coef[A->Size-1]+invBASE*(A->Coef[A->Size-2]
    + invBASE*A->Coef[A->Size-3]);
  x = BASE/x;
  B->Coef[1] = floor(x);
  B->Coef[0] = floor((x-B->Coef[1])*BASE);
  B->Size = 2;

  /* Iteration used : B <- B+(1-AB)*B */
  N=2;
  while (N<A->Size) {
    /* Use only the first 2*N limbs of A */
    NN = 2*N;
    if (NN>A->Size) 
      NN = A->Size;
    AA.Coef = A->Coef+A->Size-NN;
    AA.Size = NN;
    MulBigInt(&AA,B,tmpBigInt);
    Delta = NN+B->Size-1;
    /* Compute BASE^Delta-tmpBigInt in tmpBigInt */
    if (tmpBigInt->Size==Delta) {
      Sign=1;
      for (i=0; i<Delta; i++)
        tmpBigInt->Coef[i] = BASE-1 - tmpBigInt->Coef[i];
      UpdateBigInt(tmpBigInt);
    }
    else {
      Sign = -1;
      tmpBigInt->Coef[Delta] = 0.;
    }

    MulBigInt(tmpBigInt,B,tmpBigInt);
    for (i=0; i<tmpBigInt->Size-2*N+1; i++)
      tmpBigInt->Coef[i] = tmpBigInt->Coef[i+2*N-1];
    tmpBigInt->Size -= 2*N-1;
    for (i=B->Size-1; i>=0; i--)
      B->Coef[i+NN-N] = B->Coef[i];
    for (i=NN-N-1; i>=0; i--)
      B->Coef[i]=0.;
    B->Size += NN-N;
    if (Sign==-1) {
      for (i=0; i<tmpBigInt->Size; i++)
        tmpBigInt->Coef[i] = -tmpBigInt->Coef[i];
    }
    AddBigInt(B,tmpBigInt,B);
    /* Once during the process, redo one iteration with same precision */
    if (8*N>A->Size && Twice) {
      Twice=0;
      B->Size = N;
      for (i=0; i<N; i++)
        B->Coef[i] = B->Coef[i+N];
    }
    else
      N *= 2;
  }
}