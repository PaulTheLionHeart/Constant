/*
** Chatty (ChatGPT) and LionHeart: July 2026
**
** Subject:
**
** Apery's Constant, zeta(3)
**
** This program computes Apery's Constant to many decimal places.
**
** Based on the rapidly convergent zeta(3) series used by
** Stefan Spannare in sszeta3.c, version 1.03, 2007-08-13.**
**	https://www.spaennare.se/ssprog.html
**
** Formula:
**
**    (To be added once we choose the final recurrence.)
**
*/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>
#include "maths.h"

/*
** Computation of Apery's Constant, zeta(3)
*/
void AperyMain (HWND hwnd, char *output, char *name, char *Time)
    {
    clock_t endclock, startclock; 
    long size=1+NbDigits/LB;
    long *vk = (long *)malloc(size * sizeof(long));
    long *uk = (long *)malloc(size*sizeof(long));
    long k = 0;
//    char s[120];

    SetWindowText (hwnd, "Initialising");
    startclock = clock();    

    /*
    ** Apéry's Constant zeta(3)
    **
    ** Stefan Spännare Method 1:
    **
    ** zeta(3) = 1/64 * SUM (-1)^n
    **           (205n^2 + 250n + 77) * (n!)^10 / ((2n+1)!)^5
    */

    SetToInteger(size, uk, 77);      // current term T0
    SetToInteger(size, vk, 77);      // running sum = +T0

    for (__int64 n = 0; ; n++)
	{
	__int64 p0 = 205 * n * n + 250 * n + 77;
	__int64 p1 = 205 * n * n + 660 * n + 532;

	/*
	** Advance uk from Tn to T(n+1)
	*/
	Mul64(size, uk, p1);
	DivX64(size, uk, p0);

	for (int i = 0; i < 5; i++)
	    Mul64(size, uk, n + 1);

	for (int i = 0; i < 5; i++)
	    DivX64(size, uk, 2);

	for (int i = 0; i < 5; i++)
	    DivX64(size, uk, 2 * n + 3);

	/*
	** Stop once the term has vanished at requested precision.
	** Replace IsZero with your existing zero-test if named differently.
	*/
	if (IsZero(size, uk))
	    break;

	/*
	** Series signs:
	** +T0 -T1 +T2 -T3 ...
	**
	** Since uk has already advanced to T(n+1):
	** n=0 -> subtract T1
	** n=1 -> add T2
	*/
	if ((n & 1) == 0)
	    Sub(size, vk, uk);
	else
	    Add(size, vk, uk);
	}

    /*
    ** Final factor 1/64.
    */
    DivX64(size, vk, 64);

//    Print(size, vk, output);
    
    SetWindowText (hwnd, "Generating Output");
    sprintf_s (output, 100, "%d.", vk[0]);
//    sprintf_s (name, 100, "Computation time is : %9.2f seconds. Constant Square Root of 2 = %d.\n", (float)(endclock-startclock)/(float)CLOCKS_PER_SEC, r2[0]);
    sprintf_s(name, 100, "Apery's Constant = ");
    Print(size, vk, output);
    endclock = clock ();
    sprintf_s (Time, 100, "Computation time is : %9.2f seconds\n",  (float)(endclock-startclock)/(float)CLOCKS_PER_SEC ); 
    SetWindowText (hwnd, "Paul's Constant Generator");
    free (vk);
    free (uk);
    }



