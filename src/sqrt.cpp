/*
** Pascal Sebah : July 1999
** 
** Subject:
**
**    A very easy program to compute sqrt(2) with many digits.
**    No optimisations, no tricks, just a basic program to learn how
**    to compute in multiprecision.  
**
** Formula:
**
**    sqrt(2) = (239/169)*1/sqrt(1-1/57122)
**
** Data:
**
**    A big real (or multiprecision real) is defined in base B as:
**      X = x(0) + x(1)/B^1 + ... + x(n-1)/B^(n-1)
**      where 0<=x(i)<B
**
** Results: (PentiumII, 450Mhz)
**    
**    1000   decimals :   0.02seconds
**    10000  decimals :   1.7s
**    100000 decimals : 176.0s
**
** With a little work it's possible to reduce those computation
** times by a factor of 3 and more.
*/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include "maths.h"

extern const SqrtData SqrtTable[];

#include <stdio.h>
#include <math.h>
#include <limits.h>

// ---------------------------------------------------------------------------
// GenerateTable()
//
// Development tool used while generalising Pascal Sebah's Sqrt2 algorithm
// into the generic SqrtMain() implementation.
//
// It searches for rational approximations p/q satisfying:
//
//     n*q² - p² = x
//
// and was invaluable for constructing and experimenting with the
// SqrtTable[] entries for sqrt(2) through sqrt(20).
//
// The final table has now been verified against independent references,
// so this routine is no longer required for normal operation, but is
// retained for future experimentation.
// ---------------------------------------------------------------------------

#ifdef GENROOTTABLE
int GenerateSqrtTable(void)
    {
    char s[200];

    for (int index = 2; index <= 20; index++)
	{
	__int64 bestResidual = _I64_MAX;
	long bestP = 0;
	long bestQ = 0;

	for (long q = 1; q <= 500; q++)
	    {
	    long p = (long)(sqrt((double)index) * (double)q + 0.5);

	    __int64 qSquared = (__int64)q * (__int64)q;

	    __int64 residual =
		(__int64)index * qSquared -
		(__int64)p * (__int64)p;

	    // We need p² < index*q²
	    if (residual <= 0)
		continue;

	    if (residual < bestResidual)
		{
		bestResidual = residual;
		bestP = p;
		bestQ = q;
		}
	    else if (residual == bestResidual && q > bestQ)
		{
		bestP = p;
		bestQ = q;
		}
	    }

	sprintf_s(s,
	    "// sqrt(%d)\r\n",
	    index);
	OutputDebugStringA(s);

	sprintf_s(s,
	    "{ %ld, %ld, %lld, %lld },\r\n\r\n",
	    bestP,
	    bestQ,
	    bestResidual,
	    (__int64)index * (__int64)bestQ * (__int64)bestQ);

	OutputDebugStringA(s);
	}

    return 0;
    }

#endif // GENROOTTABLE


/*
** Computation of the constant sqrt(2)
*/
void SqrtMain (HWND hwnd, char *output, char *name, char *Time, int index)
    {
    clock_t endclock, startclock; 
    long size=1+NbDigits/LB;
    long *r2 = (long *)malloc(size*sizeof(long));
    long *uk = (long *)malloc(size*sizeof(long));
    long k=1;
    char s[120];

#ifdef GENROOTTABLE
    GenerateSqrtTable();
#endif // GENROOTTABLE

  /*
  ** Formula used :
  **    sqrt(2) = (239/169)*1/sqrt(1-1/57122)
  ** and
  **   1/sqrt(1-x) = 1+(1/2)x+(1.3)/(2.4)x^2+(1.3.5)/(2.4.6)x^3+...
  */
    SetWindowText (hwnd, "Initialising");
    startclock = clock();    

    if (index == 4 || index == 9 || index == 16)
	{
	int value = 0;

	switch (index)
	    {
	    case 4:  value = 2; break;
	    case 9:  value = 3; break;
	    case 16: value = 4; break;
	    }

	SetToInteger(size, r2, value);
	sprintf_s(name, 100, "Constant Square Root of %d = ", index);

	sprintf_s(output, 100, "%d.\n", r2[0]);
	Print(size, r2, output);

	endclock = clock();
	sprintf_s(Time, 100, "Computation time is : %9.2f seconds\n", (float)(endclock - startclock) / (float)CLOCKS_PER_SEC);
	SetWindowText(hwnd, "Paul's Constant Generator");
	return;
	}

    SetToInteger (size, r2, 1); /* r2 = 1 */
    SetToInteger (size, uk, 1);	/* uk = 1 */

    while (!IsZero(size, uk))
	{
	sprintf(s, "Calculating series: pass %d", k);
	SetWindowText(hwnd, s);
	Mul(size, uk, SqrtTable[index].xNumerator);
	DivX(size, uk, SqrtTable[index].xDenominator);
	DivX(size, uk, 2 * k);
	Mul(size, uk, 2 * k - 1);
	Add(size, r2, uk);    // r2 = r2+uk
	k++;
	}
    SetWindowText(hwnd, "Generating Output");
    Mul(size, r2, SqrtTable[index].numerator);
    DivX(size, r2, SqrtTable[index].denominator);

    sprintf_s (output, 100, "%d.", r2[0]);
//    sprintf_s (name, 100, "Computation time is : %9.2f seconds. Constant Square Root of 2 = %d.\n", (float)(endclock-startclock)/(float)CLOCKS_PER_SEC, r2[0]);
    sprintf_s (name, 100, "Constant Square Root of %d = ", index);
    Print (size, r2, output);  /* Print out of Sqrt2 */
    endclock = clock ();
    sprintf_s (Time, 100, "Computation time is : %9.2f seconds\n",  (float)(endclock-startclock)/(float)CLOCKS_PER_SEC ); 
    SetWindowText (hwnd, "Paul's Constant Generator");
    free (r2);
    free (uk);
    }



