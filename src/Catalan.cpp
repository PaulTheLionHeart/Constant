/*
** Chatty (ChatGPT) and LionHeart: July 2026
** 
** Subject:
**
**    A very easy program to compute Catalan's Constant (G) with many digits.
**    No optimisations, no tricks, just a basic program to learn how
**    to compute in multiprecision.  
**
** Formula used :
**
**    Catalan's Constant computed using the rapidly
**    convergent series published by Stefan Spännare.
**
**    The factorial terms are replaced by a simple
**    recurrence using only integer multiplication
**    and division.
**
**    Catalan's Constant (for those who like equations):
**
**      G = (1/64) * Sum (-1)^(n-1)
**
**          2^(8n) (40n^2-24n+3) (2n)!^3 (n!)^2
**        ---------------------------------------
**        n^3 (2n-1) ((4n)!)^2
**
**    The factorial expression is evaluated using
**    a recurrence to avoid explicit factorials.
**
**    Reference:
**
**      Stefan Spännare,
**      "Catalan's Constant to Many Decimal Places",
**      Version 1.02 (2007)
**	https://www.spaennare.se/ssprog.html
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>
#include "maths.h"

/********************************************
    Computation of the Catalan's Constant (G)
********************************************/

void CatalanMain(HWND hwnd, char *output, char *name, char *Time)
    {
    clock_t endclock, startclock;

    long size = 1 + NbDigits / LB;

    long *G = (long *)malloc(size * sizeof(long));
    long *uk = (long *)malloc(size * sizeof(long));

    char s[120];

    SetWindowText(hwnd, "Initialising");
    startclock = clock();

    SetToInteger(size, G, 0);

    // First term is 19/18
    SetToInteger(size, uk, 19);
    DivX64(size, uk, 18);

    long k = 1;
    int sign = 1;

    while (!IsZero(size, uk))
	{
	sprintf_s(s, "Calculating Catalan series: pass %ld", k);
	SetWindowText(hwnd, s);

	if (sign > 0)
	    Add(size, G, uk);
	else
	    Sub(size, G, uk);

	__int64 A = 40 * (__int64)k * k - 24 * (__int64)k + 3;
	__int64 A2 = 40 * (__int64)k * k + 56 * (__int64)k + 19;

	// u(k+1) = u(k) *
	// 32*k^3*(2k-1)*(40k^2+56k+19)
	// /
	// ((40k^2-24k+3)*(4k+1)^2*(4k+3)^2)

	Mul64(size, uk, 32);

	Mul64(size, uk, k);
	DivX64(size, uk, 4 * (__int64)k + 1);

	Mul64(size, uk, k);
	DivX64(size, uk, 4 * (__int64)k + 1);

	Mul64(size, uk, k);
	DivX64(size, uk, 4 * (__int64)k + 3);

	Mul64(size, uk, 2 * (__int64)k - 1);
	DivX64(size, uk, 4 * (__int64)k + 3);

	Mul64(size, uk, A2);
	DivX64(size, uk, A);

	k++;
	sign = -sign;
	if (k > 5000)
	    {
	    OutputDebugStringA("Exit before infinite loop\n");
	    break;
	    }
	}

    SetWindowText(hwnd, "Generating Output");

    sprintf_s(output, 100, "%d.", G[0]);
    sprintf_s(name, 100, "Catalan's Constant (G) = ");

    Print(size, G, output);

    endclock = clock();

    sprintf_s(Time,
	100,
	"Computation time is : %9.2f seconds\n",
	(float)(endclock - startclock) / (float)CLOCKS_PER_SEC);

    SetWindowText(hwnd, "Paul's Constant Generator");

    free(G);
    free(uk);
    }


