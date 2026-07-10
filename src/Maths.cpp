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
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "maths.h"

long B=10000; /* Working base */
long LB=4;    /* Log10(base)  */
long MaxDiv=450;  /* about sqrt(2^31/B) */

//extern	long NbDigits;
/*
** Set the big real x to the small integer Integer 
*/
void SetToInteger (long n, long *x, long Integer) {
  long i;
  for (i=1; i<n; i++) x[i] = 0;
  x[0] = Integer;
}
/*
** Is the big real x equal to zero ?
*/
long IsZero (long n, long *x) {
  long i;
  for (i=0; i<n; i++)  
    if (x[i])	return 0;
	return 1;
}
/*
** Addition of big reals : x += y
**  Like school addition with carry management
*/
void Add (long n, long *x, long *y) {
  long carry=0, i;
  for (i=n-1; i>=0; i--) {
    x[i] += y[i]+carry;
    if (x[i]<B) carry = 0;
    else {
      carry = 1;
      x[i] -= B;
    }
  }  
}
/*
** Substraction of big reals : x -= y
**  Like school substraction with carry management
**  x must be greater than y
*/
void Sub (long n, long *x, long *y) {
  long i;
  for (i=n-1; i>=0; i--) {
    x[i] -= y[i];
		if (x[i]<0) {
		  if (i) {	
        x[i] += B;
        x[i-1]--;
      }
		}
  }  
}
/*
** Multiplication of the big real x by the integer q 
** x = x*q.
**  Like school multiplication with carry management
*/
void	Mul(long n, long *x, long q)
    {
    __int64 carry = 0;
    __int64 xi;
    long i;

    for (i = n - 1; i >= 0; i--)
	{
	xi = (__int64)x[i] * (__int64)q;
	xi += carry;

	carry = xi / B;
	x[i] = (long)(xi % B);
	}
    }
/*
** Division of the big real x by the integer d 
** The result is y=x/d.
**  Like school division with carry management
**  d is limited to MaxDiv*MaxDiv.
*/
void Div(long n, long *x, long d, long *y)
    {
    long i;
    long carry = 0;

    for (i = 0; i < n; i++)
	{
	__int64 xi;

	xi = (__int64)x[i] +
	    (__int64)carry * (__int64)B;

	y[i] = (long)(xi / d);
	carry = (long)(xi % d);
	}
    }

/*
** Division of the big real x by the integer d 
** The result is x=x/d.
**  Like school division with carry management
*/

void DivX(long n, long *x, long d)
    {
    long i;
    long carry = 0;

    for (i = 0; i < n; i++)
	{
	__int64 xi;

	xi = (__int64)x[i] +
	    (__int64)carry * (__int64)B;

	x[i] = (long)(xi / d);

	carry = (long)(xi % d);
	}
    }

/*
** Find the arc cotangent of the integer p = arctan (1/p)
**  Result in the big real x (size n)
**  buf1 and buf2 are two buffers of size n
*/
void arccot (long p, long n, long *x, long *buf1, long *buf2) {
  long p2=p*p, k=3, sign=0;
  long *uk=buf1, *vk=buf2;
  SetToInteger (n, x, 0);
  SetToInteger (n, uk, 1);	/* uk = 1/p */
  Div (n, uk, p, uk);
  Add (n, x, uk);	          /* x  = uk */

  while (!IsZero(n, uk)) {
    if (p<MaxDiv)
      Div (n, uk, p2, uk);  /* One step for small p */
    else {
      Div (n, uk, p, uk);   /* Two steps for large p (see division) */
      Div (n, uk, p, uk);  
    }
    /* uk = u(k-1)/(p^2) */
    Div (n, uk, k, vk);       /* vk = uk/k  */
    if (sign) Add (n, x, vk); /* x = x+vk   */
    else Sub (n, x, vk);      /* x = x-vk   */
    k+=2;
    sign = 1-sign;
  }
}

/*
** Find the arc hyperbolic tangent of the integer p = arctan (1/p)
**     atanh(x) = 1/2*Log((1+x)/(1-x)) = x + x^3/3 + x^5/5 + ...
**     and x=1/p 
**  Result in the big real x (size n)
**  buf1 and buf2 are two buffers of size n
*/
void arctanh (long p, long n, long *x, long *buf1, long *buf2) 
    {
    long p2=p*p, k=3, sign=0;
    long *uk=buf1, *vk=buf2;
    SetToInteger (n, x, 1);		// x = 1/p
    Div (n, x, p, x);
    SetToInteger (n, uk, 1);		// uk = 1/p
    Div (n, uk, p, uk);

    while (!IsZero(n, uk)) 
	{
	Div (n, uk, p2, uk);   /* uk = u(k-1)/(p^2) */
	Div (n, uk, k, vk);    /* vk = uk/k         */
	Add (n, x, vk);       /* l2 = l2+vk        */
	k+=2;
	}
    }

/*
** Print the big real x
*/
void Print (long n, long *x, char *buffer) 
    {
    long i; 
    char	buf[80];

//    *buffer = '\0';
    for (i=1; i<n; i++) 
	{
	sprintf_s (buf, 80, "%.4d", x[i]);
	strcat_s(buffer, NbDigits*2-1, buf);
	}
    strcat_s (buffer, NbDigits*2-1, "\n");
    }

/*
** 64-bit versions for larger integer multipliers and divisors.
** Used by some of the newer constant generators.
*/

/*
** Multiplication of the big real x by the integer q
** x = x*q.
**  Like school multiplication with carry management
*/
void Mul64(long n, long *x, __int64 q)
    {
    __int64 carry = 0;

    for (long i = n - 1; i >= 1; i--)
	{
	__int64 xi = (__int64)x[i] * q + carry;
	carry = xi / B;
	x[i] = (long)(xi % B);
	}

    x[0] = (long)((__int64)x[0] * q + carry);
    }

/*
** Division of the big real x by the integer d
** The result is x=x/d.
**  Like school division with carry management
*/

void DivX64(long n, long *x, __int64 d)
    {
    __int64 carry = 0;

    for (long i = 0; i < n; i++)
	{
	__int64 xi = (__int64)x[i] + carry * B;
	x[i] = (long)(xi / d);
	carry = xi % d;
	}
    }

