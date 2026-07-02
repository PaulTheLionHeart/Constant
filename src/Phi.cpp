/*
    Thanks to the elementary programs described in the page 
    http://numbers.computation.free.fr/Constants/TinyPrograms/tinycodes.html
    Tiny programs it is possible to perform an online computation of some 
    constants by a small program. 

    Tiny algorithm for a constant defined by an hypergeometric
    series :  Pascal Sebah (2000)
    4*DIGITS is the number of digits.
    Each term of the serie is multiplied by (n1*k+n2)/(d1*k+d2).
    r is about 1/log10(d1/n1) and B is the working base.
*/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include "maths.h"

void HyperGeom (HWND hwnd, char *digits, int I, int d1, int d2, int n1, int n2, double r) 
    {
  // digits will contain all the digits
    long    D = NbDigits/LB;
    long    S = (long)(floor(4.0 * r * (double)D));
    long    i, k, l, d, p = NbDigits/LB/1000;
    long    *b;
    char    buf[80];

    *digits = '\0';
    b = new long [S+1];
    for (i=0;i<=S; i++) 
	b[i] = I;
    for (;D--;) 
	{
	if ((D & 1000) == 0)
	    {
	    sprintf (buf, "Calculating: pass %d of %d", p - D/1000, p);	
	    SetWindowText (hwnd, buf);
	    }
	for (l=0, k=S; --k;) 
	    {
	    b[k] = (l+=b[k]*B)%(i=d1*k+d2);
//	    l  = floor(l/i);
	    l  = l / i;
	    if (n1>0)
		l *= (n1*k+n2);
	    }
	l += b[0]*B;
	b[0] = l%B;
//	d = floor(l/B);
	d = l / B;
	sprintf(buf, "%04d", d);
	strcat(digits, buf);
/*
	if (d==0)
	    {
	    strcat(digits, "0000");
//	    digits = digits+'0000'; 
	    }
	else if (d<10)
	    {
	    sprintf(buf, "000%d", d);
	    strcat(digits, buf);
	    }
//	    digits = digits+'000'+d;
	else if (d<100)
	    {
	    sprintf(buf, "00%d", d);
	    strcat(digits, buf);
	    }
//	    digits = digits+'00'+d;    
	else if (d<1000)
	    {
	    sprintf(buf, "0%d", d);
	    strcat(digits, buf);
	    }
//	    digits = digits+'0'+d;    
	else
	    {
	    sprintf(buf, "%d", d);
	    strcat(digits, buf);
	    }
//	    digits = digits+d;
*/
	}
    //  return digits;
    delete [] b;
    } 

/*
 // Computation of constants
 function PiC ()  {
   digits = HyperGeom (2, 2, 1, 1, 0, 3.33);
   pfinal = '3.'+digits.slice (4, digits.length)+'...';
   PrintD (pfinal);
 }
 function eC ()  {
   digits = HyperGeom (1, 1, 0, 0, 0, 1.00);
   pfinal = '2.'+digits.slice (4, digits.length)+'...';
   PrintD (pfinal);
 }
 function phiC ()  {
   digits = HyperGeom (10, 10, 0, 2, -1, 1.44);
   pfinal = '1.6'+digits.slice (4, digits.length)+'...';
   PrintD (pfinal);
 }
 function sqrt2C ()  {
   digits = HyperGeom (14, 100, 0, 2, -1, 0.59);
   pfinal = '1.'+digits.slice (3, digits.length)+'...';
   PrintD (pfinal);
 }
 function log2C ()  {
   digits = HyperGeom (5, 2, 2, 1, 0, 3.33);
   pfinal = '0.'+digits.slice (3, digits.length)+'...';
   PrintD (pfinal);
 }
*/

/*
** Computation of the constant Log(3)
*/

void PhiMain (HWND hwnd, char *output, char *name, char *Time) 
// function log3C ()  {
    {
    char	*buffer;

    buffer = new char [NbDigits + 1];
    clock_t endclock, startclock; 
    long size=1+NbDigits/LB;

    startclock = clock();    
    HyperGeom (hwnd, buffer, 10, 10, 0, 2, -1, 1.44);
    SetWindowText (hwnd, "Generating Output");
    sprintf(output, "1.6");
    strcpy(output + 3, buffer + 4);		    // discard the first 4 digits. Not sure why 
    sprintf_s (name, 100, "Constant Phi = ");
    endclock = clock ();
    sprintf_s (Time, 100, "Computation time is : %9.2f seconds\n",  (float)(endclock-startclock)/(float)CLOCKS_PER_SEC ); 
//   pfinal = '1.'+digits.slice (3, digits.length)+'...';
//   PrintD (pfinal);
    delete [] buffer;
    SetWindowText (hwnd, "Paul's Constant Generator");
    }
 
/*
** Computation of the constant Log(3)
*/
/*
void Log3Main1 (char *output, char *name, char *Time) 
// function log3C ()  {
    {
    clock_t endclock, startclock; 
    long size=1+NbDigits/LB;

    startclock = clock();    
    HyperGeom (output, 10, 8, 4, 2, -1, 1.67);
    sprintf_s (name, 100, "Constant Log 3 = 1.\n");
//    Print (size, l2, output);  // Print out of Log2
    endclock = clock ();
    sprintf_s (Time, 100, "Computation time is : %9.2f seconds\n",  (float)(endclock-startclock)/(float)CLOCKS_PER_SEC ); 
//   pfinal = '1.'+digits.slice (3, digits.length)+'...';
//   PrintD (pfinal);
    }
 */

/*
 // Change the number of computed digits
 function D50 () {
   DIGITS = 13;
 }
 function D100 () {
   DIGITS = 26;
 }
 function D250 () {
   DIGITS = 63;
 }
 function D500 () {
   DIGITS = 126;
 } 
 function D1000 () {
   DIGITS = 251;
 }
 // Printing
 function PrintD (toPrint) {
   var i, S=50, L = Math.floor(toPrint.length/S), section='';
   for (i=0;i<toPrint.length; i+=S) 
     section = section+toPrint.slice (i, i+S)+'\n';
   document.forms[0].zone.value = section;  
 }
  
*/

