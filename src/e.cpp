/*
** Pascal Sebah : July 1999
** 
** Subject:
**
**    A very easy program to compute e with many digits.
**    No optimisations, no tricks, just a basic program to learn how
**    to compute in multiprecision.  
**
** Formula:
**
**    e = 1+1/1!+1/2!+...+1/k!+...
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
**    10000  decimals :   1.2s
**    100000 decimals :  97.0s
**    200000 decimals : 375.0s
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
** Computation of the constant e
*/
void eMain (HWND hwnd, char *output, char *name, char *Time) 
    {
    clock_t endclock, startclock; 
    long size=1+NbDigits/LB;
    long *e  = (long *)malloc(size*sizeof(long));
    long *uk = (long *)malloc(size*sizeof(long));
    long k=1;
    char s[80];
  /*
  ** Formula used : e = 1+1/1!+1/2!+...+1/k!+...
  */ 
    SetWindowText (hwnd, "Initialising");
    startclock = clock();    
    SetToInteger (size, e,  1); /* e  = 1 */
    SetToInteger (size, uk, 1);	/* uk = 1 */
    while (!IsZero(size, uk)) 
	{
	sprintf (s, "Calculating series: pass %d", k);	
	SetWindowText (hwnd, s);
	DivX (size, uk, k);  /* uk = u(k-1)/k */
	Add (size, e,  uk); /* e  = e + uk */
	k++;
	}  
    endclock = clock ();

    SetWindowText (hwnd, "Generating Output");
    sprintf_s (output, 100, "%d.", e[0]);
    sprintf_s (name, 100, "Constant e = ");
    Print (size, e, output);  /* Print out of e */
    sprintf_s (Time, 100, "Computation time is : %9.2f seconds\n",  (float)(endclock-startclock)/(float)CLOCKS_PER_SEC ); 
    SetWindowText (hwnd, "Paul's Constant Generator");
    free (e);
    free (uk);
    }



