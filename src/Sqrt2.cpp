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
#include <windows.h>
#include "maths.h"

/*
** Computation of the constant sqrt(2)
*/
void SqrtMain (HWND hwnd, char *output, char *name, char *Time) 
    {
    clock_t endclock, startclock; 
    long size=1+NbDigits/LB;
    long *r2 = (long *)malloc(size*sizeof(long));
    long *uk = (long *)malloc(size*sizeof(long));
    long k=1;
    char s[120];

  /*
  ** Formula used :
  **    sqrt(2) = (239/169)*1/sqrt(1-1/57122)
  ** and
  **   1/sqrt(1-x) = 1+(1/2)x+(1.3)/(2.4)x^2+(1.3.5)/(2.4.6)x^3+...
  */
    SetWindowText (hwnd, "Initialising");
    startclock = clock();    
    SetToInteger (size, r2, 1); /* r2 = 1 */
    SetToInteger (size, uk, 1);	/* uk = 1 */
    while (!IsZero(size, uk)) 
	{
	sprintf (s, "Calculating series: pass %d", k);	
	SetWindowText (hwnd, s);
	DivX (size, uk, 57122); /* uk = u(k-1)/57122 * (2k-1)/(2k) */
	DivX (size, uk, 2*k);  
	Mul (size, uk, 2*k-1);  
	Add (size, r2, uk);    /* r2 = r2+uk */
	k++;
	}
    SetWindowText (hwnd, "Generating Output");
    Mul (size, r2, 239);  
    DivX (size, r2, 169);  /* r2 = (239/169)*r2 */

    sprintf_s (output, 100, "%d.\n", r2[0]);
//    sprintf_s (name, 100, "Computation time is : %9.2f seconds. Constant Square Root of 2 = %d.\n", (float)(endclock-startclock)/(float)CLOCKS_PER_SEC, r2[0]);
    sprintf_s (name, 100, "Constant Square Root of 2 = ");
    Print (size, r2, output);  /* Print out of Sqrt2 */
    endclock = clock ();
    sprintf_s (Time, 100, "Computation time is : %9.2f seconds\n",  (float)(endclock-startclock)/(float)CLOCKS_PER_SEC ); 
    SetWindowText (hwnd, "Paul's Constant Generator");
    free (r2);
    free (uk);
    }



