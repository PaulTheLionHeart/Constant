/*
** Pascal Sebah : September 1999
** 
** Subject:
**
**    A very easy program to compute Pi with many digits.
**    No optimisations, no tricks, just a basic program to learn how
**    to compute in multiprecision.  
**
** Formulae:
**
**    Pi/4 =    arctan(1/2)+arctan(1/3)                     (Hutton 1)
**    Pi/4 =  2*arctan(1/3)+arctan(1/7)                     (Hutton 2)
**    Pi/4 =  4*arctan(1/5)-arctan(1/239)                   (Machin)
**    Pi/4 = 12*arctan(1/18)+8*arctan(1/57)-5*arctan(1/239) (Gauss)
**
**      with arctan(x) =  x - x^3/3 + x^5/5 - ...
**
**    The Lehmer's measure is the sum of the inverse of the decimal
**    logarithm of the pk in the arctan(1/pk). The more the measure
**    is small, the more the formula is efficient.
**    For example, with Machin's formula:
**
**      E = 1/log10(5)+1/log10(239) = 1.852
** 
** Data:
**
**    A big real (or multiprecision real) is defined in base B as:
**      X = x(0) + x(1)/B^1 + ... + x(n-1)/B^(n-1)
**      where 0<=x(i)<B
**
** Results: (PentiumII, 450Mhz)
**    
**   Formula      :    Hutton 1  Hutton 2   Machin   Gauss
**   Lehmer's measure:   5.418     3.280      1.852    1.786
**
**  1000   decimals:     0.2s      0.1s       0.06s    0.06s
**  10000  decimals:    19.0s     11.4s       6.7s     6.4s
**  100000 decimals:  1891.0s   1144.0s     785.0s   622.0s
**
** With a little work it's possible to reduce those computation
** times by a factor 3 and more:
**  
**     => Work with double instead of long and the base B can
**        be choosen as 10^8
**     => During the iterations the numbers you add are smaller
**        and smaller, take this in account in the +, *, /
**     => In the division of y=x/d, you may precompute 1/d and
**        avoid multiplications in the loop (only with doubles)
**     => MaxDiv may be increased to more than 3000 with doubles
**     => ...
*/
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <windows.h>
#include "maths.h"

/*
** Computation of the constant Pi with arctan relations
*/
void PiMain (HWND hwnd, char *output, char *name, char *Time) 
    {  
    clock_t endclock, startclock; 
    long NbArctan;
    long p[10], m[10];
    long size=1+NbDigits/LB, i;
//    char	buf[80];
    long *Pi      = (long *)malloc(size*sizeof(long));
    long *arctan  = (long *)malloc(size*sizeof(long));
    long *buffer1 = (long *)malloc(size*sizeof(long));
    long *buffer2 = (long *)malloc(size*sizeof(long)); 
    char s[120];

  /*
  ** Formula used: 
  **   
  **   Pi/4 = 12*arctan(1/18)+8*arctan(1/57)-5*arctan(1/239) (Gauss)
  */
    startclock = clock();    
    NbArctan = 3;
    m[0] = 12; m[1] = 8;  m[2] = -5;
    p[0] = 18; p[1] = 57; p[2] = 239; 

    SetWindowText (hwnd, "Initialising");
    SetToInteger (size, Pi, 0);
  /*
  ** Computation of Pi/4 = Sum(i) [m[i]*arctan(1/p[i])] 
  */
    for (i=0; i<NbArctan; i++) 
	{
	sprintf (s, "Calculating arc cotangent: pass %d of %d", i + 1, NbArctan);	
	SetWindowText (hwnd, s);
	arccot (p[i], size, arctan, buffer1, buffer2);
	Mul (size, arctan, abs(m[i]));
	if (m[i]>0) Add (size, Pi, arctan);  
	else        Sub (size, Pi, arctan);  
	}
    SetWindowText (hwnd, "Generating Output");
    Mul (size, Pi, 4);

    sprintf_s (output, 100, "%d.", Pi[0]);
    sprintf_s (name, 100, "Constant Pi = ");
    Print (size, Pi, output);  /* Print out of Pi */
    endclock = clock ();
    sprintf_s (Time, 100, "Computation time is : %9.2f seconds\n",  (float)(endclock-startclock)/(float)CLOCKS_PER_SEC ); 
    SetWindowText (hwnd, "Paul's Constant Generator");
    free (Pi);
    free (arctan);
    free (buffer1);
    free (buffer2);
    }

