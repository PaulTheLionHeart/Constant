/***********************************************************************/
/*                                                                     */
/*                                                                     */
/* Program:                                                            */
/* --------                                                            */
/*                                                                     */
/* sszeta3.c                                                           */
/*                                                                     */
/*                                                                     */
/* Purpose:                                                            */
/* --------                                                            */
/*                                                                     */
/* This program calculates Apery's constant Zeta(3) to many            */ 
/* decimal digits.                                                     */
/*                                                                     */
/*                                                                     */
/* Version 1.03:                                                       */
/* -------------                                                       */
/*                                                                     */
/* First version: July 2003                                            */
/* Latest update: 2007-08-13                                           */
/*                                                                     */
/*                                                                     */
/* Author of the program:                                              */
/* ----------------------                                              */
/*                                                                     */
/* Stefan Spaennare, Lund, Sweden                                      */
/*                                                                     */
/* E-mail: stefan@spaennare.se                                         */
/*                                                                     */
/* Home page:    http://www.spaennare.se/index.html                    */
/* Program page: http://www.spaennare.se/ssprog.html                   */
/*                                                                     */
/* If you have problems with the program please let me know!           */
/*                                                                     */
/*                                                                     */
/* References:                                                         */
/* -----------                                                         */
/*                                                                     */
/* 1. Some of the algorithms are found on the web-page "Mathematical   */
/* constants and computation" by Xavier Gourdon and Pascal Sebah:      */
/*                                                                     */
/* http://numbers.computation.free.fr/Constants/constants.html         */
/*                                                                     */
/* 2. The web-page "Fast Algorithms and the FEE Method",               */
/* by Ekatherina A. Karatsuba:                                         */
/*                                                                     */
/* http://www.ccas.ru/personal/karatsuba/algen.htm                     */
/*                                                                     */
/* 3. The web-page "The Karatsuba Method 'Divide and Conquer'",        */
/* by Ekatherina A. Karatsuba:                                         */
/*                                                                     */
/* http://www.ccas.ru/personal/karatsuba/divcen.htm                    */
/*                                                                     */
/* 4. The article "Fast evaluation of transcendental functions",       */
/* Problems of Information Transmission, vol. 27, (1991), p. 339-360,  */
/* by Ekatherina A. Karatsuba.                                         */
/*                                                                     */
/* 5. The article "Fast multiprecision evaluation of series of         */
/* rational numbers", by Bruno Haible and Thomas Papanikolaou, 1997.   */
/*                                                                     */
/* 6. The program now use a very fast FFT (fftsg_h.c) by Takuya Ooura: */  
/*                                                                     */
/* http://momonga.t.u-tokyo.ac.jp/~ooura/                              */
/*                                                                     */
/* 7. "CLN - Class Library for Numbers" by Bruno Haible et. al:        */
/*                                                                     */
/* http://www.ginac.de/CLN/                                            */
/*                                                                     */
/* 8. The documentation "Programs to Calculate some Mathematical       */
/* Constants to Large Precision", by Stefan Spaennare.                 */
/*                                                                     */
/*                                                                     */
/* Program history:                                                    */
/* ----------------                                                    */
/*                                                                     */
/* 2007-08-13:                                                         */
/* The code has been somewhat prettified and shorter (in main).        */
/* The program has the same speed as before.                           */
/*                                                                     */
/* 2003-10-09:                                                         */
/* Very minor changes.                                                 */
/*                                                                     */
/* 2003-08-27:                                                         */
/* Minor improvements to some procedures. Updated benchmarks.          */
/*                                                                     */
/* First version July 2003.                                            */
/*                                                                     */
/*                                                                     */
/* Copy-right:                                                         */
/* -----------                                                         */
/*                                                                     */
/* This program is free to copy if this information follows the        */
/* program. You are also allowed to modify the program for your        */
/* own calculations if you give a reference to the author.             */
/* See also notice below!                                              */
/*                                                                     */
/*                                                                     */
/* IMPORTANT!                                                          */
/* ----------                                                          */
/*                                                                     */
/* Please don't use the result from this program for important         */
/* purposes without doing an independent check of the result!          */
/* See also WARNING and notice below!                                  */
/*                                                                     */
/*                                                                     */
/* Algorithms:                                                         */
/* -----------                                                         */
/*                                                                     */
/*                                                                     */
/* The program uses (see also the references above):                   */
/*                                                                     */
/* * Fast series for Zeta(3).                                          */
/*                                                                     */
/* * FFT for multiplications.                                          */
/*                                                                     */
/* * 4-th order methods for inversions and divisions.                  */
/*                                                                     */
/* * 4-th order methods for square and cube roots.                     */
/*                                                                     */
/*                                                                     */
/* Apery's constant Zeta(3):                                           */
/* -------------------------                                           */
/*                                                                     */
/* Zeta(3) is defined as a special case of the Riemann Zeta function   */
/* zeta(s) for s = 3.                                                  */
/*                                                                     */
/*                                                                     */
/*           inf  1                    inf  1                          */
/* zeta(3) = SUM ---    ;    zeta(s) = SUM ---                         */
/*           n=1 n^3                   n=1 n^s                         */
/*                                                                     */
/* However a direct computation is very slow.                          */
/*                                                                     */
/*                                                                     */
/* Method = 1:                                                         */
/* -----------                                                         */
/*                                                                     */
/* Zeta(3) can be computed much faster using the following fast series:*/
/*                                                                     */
/*           1  inf (-1)^n (205n^2+250n+77) (n!)^10                    */
/* zeta(3) = -- SUM -------------------------------                    */
/*           64 n=0         [(2n+1)!]^5                                */
/*                                                                     */
/* Each term of the series adds about 3.0 digits to zeta(3).           */
/*                                                                     */
/*                                                                     */
/* Method = 2:                                                         */
/* -----------                                                         */
/*                                                                     */
/* Zeta(3) can be computed much faster using the following fast series:*/
/*                                                                     */
/*           1  inf (-1)^n A(n) [(2n+1)! (2n)! n!]^3                   */
/* zeta(3) = -- SUM --------------------------------                   */
/*           24 n=0      (3n+2)! [(4n+3)!]^3                           */
/*                                                                     */
/* A(n) = 126392n^5+412708n^4+531578n^3+336367n^2+104000n+12463        */
/*                                                                     */
/* Each term of the series adds about 5.0 digits to zeta(3).           */
/*                                                                     */
/*                                                                     */
/* Accuracy:                                                           */
/* ---------                                                           */
/*                                                                     */
/* The program calculates about 0.995*2^(power of 2) decimal digits.   */
/*                                                                     */
/* All printed digits are probably correct. The result for Zeta(3)     */
/* has been verified to 0.995*2^20 digits (for mulversion = 1).        */
/*                                                                     */
/*                                                                     */
/* FFT max error                                                       */
/* -------------                                                       */
/*                                                                     */
/* Set the variable mulversion=2 if "FFT max error" > 0.25.            */
/* This makes the program two times slower and requires more memory.   */
/*                                                                     */
/* The variable mulversion is present in the file "mulver.txt"         */
/* containing the single value 1 or 2. Default value is 1.             */
/*                                                                     */
/*                                                                     */
/* WARNING!                                                            */
/* --------                                                            */
/*                                                                     */
/* If you want to make calculations with much more than 2^24           */
/* (16 million) digits the convolutions in the FFT multiplications     */
/* can be saturated which will give erroneous results or no            */
/* convergence at all. If this occur then set mulversion=2 which       */
/* is 2 times slower than mulversion=1 but use the base 100            */
/* instead of 10000. This means that the convolution will be           */
/* saturated for a much larger number of digits (> 100 millions).      */
/*                                                                     */
/*                                                                     */
/* Compilation:                                                        */
/* ------------                                                        */
/*                                                                     */
/* The program works in Unix, Linux and Windows (DOS using DJGPP).     */
/* It is recommended that you optimize the program (i.e. use gcc -O3   */
/* option or similar option) when you compile:                         */
/*                                                                     */
/*>gcc -O3 -o sszeta3 sszeta3.c fftsg_h.c -lm                          */
/*                                                                     */
/* The time wraps around after 4295 seconds on some computers.         */
/*                                                                     */
/*                                                                     */
/* Output:                                                             */
/* -------                                                             */
/*                                                                     */
/* Output is to the file "zeta3.txt". This file name can easily be     */
/* changed.                                                            */
/*                                                                     */
/*                                                                     */
/* Timing, benchmark and memory:                                       */
/* -----------------------------                                       */
/*                                                                     */
/* The CPU-times are given for a 1400 MHz Intel Celeron computer       */
/* (100 MHz memory buss) and 512 Mbyte memory. The cache memory is     */
/* 256 kbyte at full CPU speed. The operating system was Red Hat 9     */
/* Linux with the gcc 3.22 C-compiler. The CPU load was on average     */
/* 1.00 (i.e. no other programs running).                              */
/*                                                                     */
/* The program was compiled using the line:                            */
/*                                                                     */
/*>gcc -O3 -o sszeta3 sszeta3.c fftsg_h.c -lm                          */
/*                                                                     */
/* The times have been measured for "power of 2" equals 17 and 20 with */
/* the "time" function in Linux. TC below means "Time Complexity".     */
/* The variable mulversion = 1.                                        */
/*                                                                     */
/* Intel Celeron 1400 MHz                                              */
/* ----------------------                                              */
/*                                                                     */
/* Algorithm           2^17       2^20          TC       Memory/(2^20) */
/*---------------------------------------------------------------------*/
/* Zeta(3), method 1  21.97 s   386.66  s   O(n*log(n)^3)  20.0 Mbyte  */
/* Zeta(3), method 2  17.29 s   341.97  s   O(n*log(n)^3)  21.8 Mbyte  */
/*---------------------------------------------------------------------*/
/*                                                                     */
/***********************************************************************/

/********************************************************************************/
/*                                                                              */
/* Notice                                                                       */
/* ======                                                                       */
/*                                                                              */
/* I make no warranties that this program is (1) free of errors, (2) consistent */
/* with any standard merchantability, or (3) meeting the requirements of a      */
/* particular application. This software shall not, partly or as a whole,       */
/* participate in a process, whose outcome can result in injury to a person or  */
/* loss of property. It is solely designed for analytical work. Permission to   */
/* use, copy and distribute is hereby granted without fee, providing that the   */
/* header above including this notice appears in all copies.                    */
/*                                                                              */
/*                                                            Stefan Spaennare  */
/*                                                                              */
/********************************************************************************/


#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


double timetic=(double)(CLOCKS_PER_SEC);

double pi=3.14159265358979323;

char version_string[80]="Program sszeta3.c, version 1.03, 2007-08-13";

char file_name[20]="zeta3.txt";      /* Output filename */

char mv_file_name[20]="mulver.txt";  /* Multiplication version file name */

int mulversion;                      /* Multiplication version (1 or 2) */

double bas=10000.0;
double basi=0.0001;

double fftmaxerror;

typedef struct biginttag {
   short *coef;
   int size,maxsize,exp0,sgn0;
} bigint;

bigint tmp,tmp2,tmp3,tmp4,tmp5;


void mul(d,n)
double *d;
int n;
{
   int nn,nn3,nn2,j;
   double x1,x2,y1,y2;
   
   nn=n+1;
   nn2=2*n+2;
   nn3=nn2+1;

   x1=d[0]; y1=d[1];
   x2=y1; y1=0.0; y2=0.0;
   d[0]=x1*x2-y1*y2;
   d[1]=x1*y2+y1*x2;

   for (j=3; j<=nn; j=j+2) {
      x1=0.5*(d[j-1]+d[nn2-j-1]);
      x2=0.5*(d[j-1]-d[nn2-j-1]);
      y1=0.5*(d[j]+d[nn3-j-1]);
      y2=0.5*(d[j]-d[nn3-j-1]);
      
      d[j-1]=x1*y1-y2*(-x2);
      d[j]=x1*(-x2)+y2*y1;
      d[nn2-j-1]=x1*y1-(-y2)*x2;
      d[nn3-j-1]=x1*x2+(-y2)*y1;
   } /* for j */

} /* mul */


void lzero(a,nn)
short *a;
int *nn;
{
   int i,l,il,dl,ib;

   l=nn[0]; il=nn[1]; dl=nn[2];

   ib=l-il-dl+1;

   for (i=l;i>=ib; i--) {
      a[i]=0;
   } /* for i */
   
   a[l+1]=1;

} /* lzero */


void linttol(a,man0,exp0,nn)
short *a;
int man0,exp0;
int *nn;
{
   int i,l,il,dl,ib;

   l=nn[0]; il=nn[1]; dl=nn[2];

   ib=l-il-dl+1;

   for (i=l;i>=ib; i--) {
      a[i]=0;
   } /* for i */

   a[l-il+exp0+1]=abs(man0);
  
   if (man0 >= 0) { 
      a[l+1]=1;
   }
   else {
      a[l+1]=-1;
  } /* if */

} /* linttol */

      
void linttol2(a,man0,exp0,nn)
short *a;
int man0,exp0;
int *nn;
{
   int i,l,il,dl,ib;
   int tt1,tt2,t1,t2,b;

   l=nn[0]; il=nn[1]; dl=nn[2];

   ib=l-il-dl+1;

   for (i=l;i>=ib; i--) {
      a[i]=0;
   } /* for i */

   b=abs(man0);

   if (b < 10000) {
      a[l-il+exp0+1]=b;
   }
   else {
      tt1=b % 100000000;
      tt2=b / 100000000;
      t1=tt1 % 10000;
      t2=tt1 / 10000;
      a[l-il+exp0+1]=t1;
      a[l-il+exp0+2]=t2;
      a[l-il+exp0+3]=tt2;
   } /* if */
  
   if (man0 >= 0) { 
      a[l+1]=1;
   }
   else {
      a[l+1]=-1;
  } /* if */

} /* linttol2 */


void ppp(t)
int t; 
{
  if (t > 999) {
     printf("%4d",t);
  }
  else if (t > 99) {
     printf("0%3d",t);
  } 
  else if (t > 9) {
     printf("00%2d",t);
  }
  else {
     printf("000%1d",t);
  } /* if */

} /* ppp */


void lprint(a,nn)
short *a;
int *nn;
{

   int i,j,m,l,il,dl,t;

   l=nn[0]; il=nn[1]; dl=nn[3];

   if (a[l+1]==-1) {
      printf("-");
   } /* if */

   i=l;
   while ((a[i]==0) && (i>(l-il+1))) {
      i--;
   } /* while */

   m=0;

   if (i==(l-il+1)) {

      printf("%4d.",a[i]);

      m++;

      for (j=(l-il); j>=(l-il-dl+1); j--) {
            t=a[j];
            ppp(t);
            m++;
            if ((m % 18)==0) {
              printf("\n");
            } /* if */
      } /* for j */ 
   }
   else {

      printf("%d",a[i]);
      m++;

      for (j=(i-1); j>=(l-il+1); j--) {
            t=a[j];
            ppp(t);
            m++;
            if ((m % 18)==0) {
               printf("\n");
            } /* if */
      } /* for j */ 
     
      printf(".");

      for (j=(l-il); j>=(l-il-dl+1); j--) {
            t=a[j];
            ppp(t);
            m++;
            if ((m % 18)==0) {
               printf("\n");
            } /* if */
      } /* for j */ 
   } /* else */

   printf("\n\n");

} /* lprint */


void fppp(t,outfile)
int t; 
FILE *outfile;
{
  if (t > 999) {
     fprintf(outfile,"%4d",t);
  }
  else if (t > 99) {
     fprintf(outfile,"0%3d",t);
  } 
  else if (t > 9) {
     fprintf(outfile,"00%2d",t);
  }
  else {
     fprintf(outfile,"000%1d",t);
  } /* if */

} /* fppp */


void lfprint(a,nn,outfile)
short *a;
int *nn;
FILE *outfile;
{

   int i,j,m,l,il,dl,t;

   l=nn[0]; il=nn[1]; dl=nn[3];

   if (a[l+1]==-1) {
      fprintf(outfile,"-");
   } /* if */

   i=l;
   while ((a[i]==0) && (i>(l-il+1))) {
      i--;
   } /* while */

   m=0;

   if (i==(l-il+1)) {

      fprintf(outfile,"%4d.",a[i]);

      m++;

      for (j=(l-il); j>=(l-il-dl+1); j--) {
            t=a[j];
            fppp(t,outfile);
            m++;
            if ((m % 18)==0) {
              fprintf(outfile,"\n");
            } /* if */
      } /* for j */ 
   }
   else {

      fprintf(outfile,"%d",a[i]);
      m++;

      for (j=(i-1); j>=(l-il+1); j--) {
            t=a[j];
            fppp(t,outfile);
            m++;
            if ((m % 18)==0) {
               fprintf(outfile,"\n");
            } /* if */
      } /* for j */ 
     
      fprintf(outfile,".");

      for (j=(l-il); j>=(l-il-dl+1); j--) {
            t=a[j];
            fppp(t,outfile);
            m++;
            if ((m % 18)==0) {
               fprintf(outfile,"\n");
            } /* if */
      } /* for j */ 
   } /* else */

   fprintf(outfile,"\n");

} /* lfprint */


void add(a,b,c,nn)
short *a;
short *b;
short *c;
int *nn;
{
  int i,ib,il,dl,l;

  int q,r,basint;

  l=nn[0]; il=nn[1]; dl=nn[2];

  ib=l-il-dl+1;

  basint=10000;
  r=0;

  for (i=ib; i<=l; i++) {
     q=b[i]+c[i]+r;
     if (q >= basint) {
        a[i]=q-basint;
	r=1;
     }
     else {
        a[i]=q;
	r=0;
     } /* if */
  } /* for i */

} /* add */
      

void sub(a,b,c,nn)
short *a;
short *b;
short *c;
int *nn;
{
  int i,ib,il,dl,l;

  int q,r,basint;

  l=nn[0]; il=nn[1]; dl=nn[2];

  ib=l-il-dl+1;

  basint=10000;
  r=0;

  for (i=ib; i<=l; i++) {
     q=b[i]-c[i]-r;
     if (q < 0) {
        a[i]=basint+q;
	r=1;
     }
     else {
        a[i]=q;
	r=0;
     } /* if */
  } /* for i */

} /* sub */


int labscomp(a,b,nn)
short *a;
short *b;
int *nn;
{
  int i,l,il,dl,ib;

  l=nn[0]; il=nn[1]; dl=nn[2];

  ib=l-il-dl+1;
  i=l;

  while ((a[i]==b[i]) && (i>=ib)) {
     i--;
  } /* while */

  if (a[i]>=b[i]) {
     return(1);
  }
  else {
     return(-1);
  } /* if */

} /* lasbcomp */


void ladd(a,b,c,nn)
short *a;
short *b;
short *c;
int *nn;
{
   int b2,c2,co,l;

   l=nn[0];

   co=labscomp(b,c,nn);
  
   b2=b[l+1];
   c2=c[l+1]; 

      if ((b2>0) && (c2>0)) {
         add(a,b,c,nn);
         a[l+1]=1;
      } /* if */
      if ((b2>0) && (c2<0) && (co==1)) {
         sub(a,b,c,nn);
         a[l+1]=1;
      } /* if */
      if ((b2>0) && (c2<0) && (co==-1)) {
         sub(a,c,b,nn);
         a[l+1]=-1;
      } /* if */
      if ((b2<0) && (c2>0) && (co==1)) {
         sub(a,b,c,nn);
         a[l+1]=-1;
      } /* if */
      if ((b2<0) && (c2>0) && (co==-1)) {
         sub(a,c,b,nn);
         a[l+1]=1;
      } /* if */
      if ((b2<0) && (c2<0)) {
         add(a,b,c,nn);
         a[l+1]=-1;
      } /* if */

} /* ladd */


void lsub(a,b,c,nn)
short *a;
short *b;
short *c;
int *nn;
{
   int b2,c2,co,l;

   l=nn[0];

   co=labscomp(b,c,nn);
  
   b2=b[l+1];
   c2=c[l+1]; 

      if ((b2>0) && (c2>0) && (co==1)) {
         sub(a,b,c,nn);
         a[l+1]=1;
      } /* if */
      if ((b2>0) && (c2>0) && (co==-1)) {
         sub(a,c,b,nn);
         a[l+1]=-1;
      } /* if */
      if ((b2>0) && (c2<0)) {
         add(a,b,c,nn);
         a[l+1]=1;
      } /* if */
      if ((b2<0) && (c2>0)) {
         add(a,b,c,nn);
         a[l+1]=-1;
      } /* if */
      if ((b2<0) && (c2<0) && (co==1)) {
         sub(a,c,b,nn);
         a[l+1]=-1;
      } /* if */
      if ((b2<0) && (c2<0) && (co==-1)) {
         sub(a,b,c,nn);
         a[l+1]=1;
      } /* if */

} /* lsub */


void lcmul(a,b,c,exp0,nn)
short *a;
short *b;
int c;
int exp0;
int *nn;
{
   int i,j,k,l,il,dl,ib;
   double q,r,cr;

   l=nn[0]; il=nn[1]; dl=nn[2];
   ib=l-il-dl+1;

   k=ib+exp0;

   for (i=ib; i<k; i++) {
      a[i]=0;
   } /* for i */

   cr=(double)(abs(c));
   r=0.0;

   for (j=ib; j<=l; j++) {
      q=(double)(b[j])*cr+r+0.5;
      r=(double)((int)(basi*q));
      if (k>=ib) {
         a[k]=(int)(q-bas*r);
      } /* if */
      k++;
   } /* for j */

   a[k]=(int)(r+0.5);

   k++;
   for (i=k; i<=l; i++) {
      a[i]=0;
   } /* for i */

   if (c>=0) {
      a[l+1]=b[l+1];
   }
   else {
      a[l+1]=-b[l+1];
   } /* if */

} /* lcmul */


void lcmul2(a,b,c,exp0,nn)
short *a;
short *b;
double c;
int exp0;
int *nn;
{
   int i,j,k,l,il,dl,ib;
   double q,r,cr;

   l=nn[0]; il=nn[1]; dl=nn[2];
   ib=l-il-dl+1;

   k=ib+exp0;

   for (i=ib; i<k; i++) {
      a[i]=0;
   } /* for i */

   cr=fabs(c);
   r=0.0;

   for (j=ib; j<=l; j++) {
      q=(double)(b[j])*cr+r+0.5;
      r=floor(basi*q);
      if (k>=ib) {
         a[k]=(int)(q-bas*r);
      } /* if */
      k++;
   } /* for j */

   a[k]=(int)(r+0.5);

   k++;
   for (i=k; i<=l; i++) {
      a[i]=0;
   } /* for i */

   if (c>=0.0) {
      a[l+1]=b[l+1];
   }
   else {
      a[l+1]=-b[l+1];
   } /* if */

} /* lcmul2 */


void ltoreal(a,man0,exp0,nn)
short *a;
double *man0,*exp0;
int *nn;
{
   char s0[20];
   char s1[10];
   char s2[10];
   char s3[10];
   char s4[10];

   char st2[10];
   char st3[10];
   char st4[10];

   int i,l,il,dl,ib,length,le;

   double man,ex;

   l=nn[0]; il=nn[1]; dl=nn[2];

   ib=l-il-dl+1;

   i=l;

   while ((a[i]==0) && (i>=ib)) {
      i--;
   } /* while */

   if ((i==ib) && (a[i]==0)) {
      man=0.0;
      ex=0.0;
   }
   else {
      sprintf(s1,"%d",a[i]);
      length=strlen(s1);
      ex=(double)(4*(i-(l-il+1))+length);

      sprintf(s2,"%d",a[i-1]);
      sprintf(s3,"%d",a[i-2]);
      sprintf(s4,"%d",a[i-3]);
 
      strcpy(st2,"");
      le=strlen(s2);
      for (i=1; i<=(4-le); i++) {
         strcat(st2,"0");
      } /* for i */
      strcat(st2,s2);

      strcpy(st3,"");
      le=strlen(s3);
      for (i=1; i<=(4-le); i++) {
         strcat(st3,"0");
      } /* for i */
      strcat(st3,s3);

      strcpy(st4,"");
      le=strlen(s4);
      for (i=1; i<=(4-le);i++) {
         strcat(st4,"0");
      } /* for i */
      strcat(st4,s4);

      sprintf(s0,"%s%s%s%s%s","0.",s1,st2,st3,st4);
      man=atof(s0);
   } /* if */
   
   *man0=(double)(a[l+1])*man;
   *exp0=ex;

} /* ltoreal */


void lrealtol(a,man0,exp0,nn)
short *a;
double man0,exp0;
int *nn;
{
    char s0[18];
    char s1[16];
 
    char z1[7];
    char z2[7];
    char z3[7];
    char z4[7];

    double man,ex,exp2,man2;

    int i,k,l,il,dl,ib,l1,l2,ll;

    l=nn[0]; il=nn[1]; dl=nn[2];

    ib=l-il-dl+1;

    for (i=l; i>=ib; i--) {
       a[i]=0;
    } /* for i */   

    man2=man0;
    ex=exp0;
    man=fabs(man2);

    while (man>=1.0) {
       man=man/10.0;
       ex=ex+1.0;
    } /* while */

    sprintf(s0,"%17.16f",man);
 
    for (i=2; i<=17; i++) {
       s1[i-2]=s0[i];
    } /* for i */


    exp2=fabs(ex/(double)(4));
    l1=(int)(ex/(double)(4));
    l2=(int)(4*(exp2-(double)(abs(l1))));
    
    if (ex<0) {
       l2=4-l2;
       l1=l1-1;
    } /* if */

    if (l2==0) {
       l2=4;
       l1=l1-1; 
    } /* if */

    for (i=0; i<=l2-1; i++) {
       z1[i]=s1[i];
    } /* for i */

    z1[i]='\0';

    for (i=0; i<=4-1; i++) {
       z2[i]=s1[l2+i];
    } /* for i */

    z2[i]='\0';

    k=0;

    for (i=0; i<=4-1; i++) {
       if ((l2+i+4)<=16) {
          z3[i]=s1[l2+i+4];
          k++;
       } /* if */
    } /* for i */
 
    z3[k]='\0'; 

    for (i=1; i<=4-k; i++) {
       strcat(z3,"0");
    } /* for i */

    k=0;

    for (i=0; i<=4-1; i++) {
       if ((l2+i+2*4)<=16) {
          z4[i]=s1[l2+i+2*4];
          k++;
       } /* if */
    } /* for i */
 
    z4[k]='\0'; 

    for (i=1; i<=4-k; i++) {
       strcat(z4,"0");
    } /* for i */
    
    ll=l-il+l1+1;
 
    a[ll]=atoi(z1);
    a[ll-1]=atoi(z2);
    a[ll-2]=atoi(z3);
    a[ll-3]=atoi(z4);

    if (man2>=0) {
       a[l+1]=1;
    }
    else {
       a[l+1]=-1;
    } /* if */

} /* lrealtol */


int lcompeps(a,nn)
short *a;
int *nn;
{
   int i,l,il,dl,k,ib,n,ll;

   l=nn[0]; il=nn[1]; dl=nn[2]; n=nn[3];

   ib=l-il-dl+1;

   k=0;

   ll=l-il-n;

   for (i=l; i>=ll; i--) {
      if (a[i]==0) {
         k++;
      } /* if */
   } /* for i */

   if (k==(il+n+1)) {
      return(-1);
   }
   else {
      return(1);
   } /* if */

}  /* lcompeps */


int lgetdec(a,nn)
short *a;
int *nn;
{
   int i,l,il,dl,k,ib;

   l=nn[0]; il=nn[1]; dl=nn[2];

   ib=l-il-dl+1;

   k=0;

   i=l-il;

   while ((a[i]==0) && (i>ib)) {
      k++;
      i--;
   } /* while */

   return(4*k);

}  /* lgetdec */


int lcheckdec(a,nn)
short *a;
int *nn;
{
   int i,l,il,dl,k,ib,ss;

   l=nn[0]; il=nn[1]; dl=nn[2];

   ib=l-il-dl+1;

   k=0;

   i=l-il;
   ss=a[i];

   while ((a[i]==ss) && (i>ib)) {
      k++;
      i--;
   } /* while */

   if ((i==ib) && (ss==0)) {
      k=0;
   } /* if */

   return(k);

}  /* lcheckdec */


void lass(a,b,nn)
short *a;
short *b;
int *nn;
{
  int i,l,il,dl,ib;

  l=nn[0]; il=nn[1]; dl=nn[2];
  
  ib=l-il-dl+1;

  for (i=l+1; i>=ib; i--) {
     a[i]=b[i];
  } /* for i */

} /* lass */


void lmul(a,b,c,nn)
short *a;
short *b;
short *c;
int *nn;
{


   int i,k,l,il,dl,ib,n,n2;
   int bi,b1,c1;
   int bn,cn,kkk,bnib,cnib;

   double isize,rr,xx;
   double br,bri,maxerror,temp,qqq;

   double *q;


if (mulversion == 1) {

   l=nn[0]; il=nn[1]; dl=nn[2];

   ib=l-il-dl+1;

   i=l;

   while ((b[i] == 0) && (i > ib)) {
      i--;
   } /* while */

   bnib=i;
   bn=bnib-ib;

   i=l;

   while ((c[i] == 0) && (i > ib)) {
      i--;
   } /* while */

   cnib=i;
   cn=cnib-ib;

   kkk=bn+cn;

   if (kkk > dl) {

   n=1;

   while (n < kkk) {
      n=2*n;
   } /* while */

   n2=2*n;

   q=(double *)malloc((n2+1)*sizeof(double));

   isize=1.0/sqrt((double)(n));

   for (i=0; i<=n2; i++) {
      q[i]=0.0;
   } /* for i */
 
   k=0;

   for (i=ib; i<=bnib; i++) {
      q[k]=(double)(b[i])*isize;
      k=k+2;
   } /* for i */

   k=0;

   for (i=ib; i<=cnib; i++) {
      q[k+1]=(double)(c[i])*isize;
      k=k+2;
   } /* for i */


   cdft(n2,1,q);

   mul(q,n);

   cdft(n2,-1,q);

   maxerror=0.0;

   rr=0.0;

   for (i=0; i<=n2; i=i+2) {
      qqq=q[i];
      temp=fabs(qqq-floor(qqq+0.5));
      if (temp > maxerror) {
         maxerror=temp;
      } /* if */
      xx=qqq+rr;
      rr=floor((xx+0.5)*basi);
      q[i]=xx-bas*rr; 
   } /* for i */

   if (maxerror > fftmaxerror) {
      fftmaxerror=maxerror;
   } /* if */

   k=2*dl;

   for (i=ib; i<=l; i++) {
      a[i]=0;
      if (k <= n2) {
         a[i]=(int)(q[k]+0.5);
      } /* if */
      k=k+2;
   } /* for i */

   free(q);

   }
   else {

   for (i=ib; i<=l; i++) {
      a[i]=0;
   } /* for i */

   } /* if */

   a[l+1]=b[l+1]*c[l+1]; 

}
else {

   l=nn[0]; il=nn[1]; dl=nn[2];

   ib=l-il-dl+1;

   i=l;

   while ((b[i] == 0) && (i > ib)) {
      i--;
   } /* while */

   bnib=i;
   bn=bnib-ib;

   i=l;

   while ((c[i] == 0) && (i > ib)) {
      i--;
   } /* while */

   cnib=i;
   cn=cnib-ib;

   kkk=bn+cn;

   if (kkk > dl) {

   n=1;

   while (n < kkk) {
      n=2*n;
   } /* while */

   n=2*n;
   n2=2*n;

   q=(double *)malloc((n2+1)*sizeof(double));

   br=100.0;
   bri=0.01;
   bi=100;

   isize=1.0/sqrt((double)(n));

   for (i=0; i<=n2; i++) {
      q[i]=0.0;
   } /* for i */
 
   k=0;

   for (i=ib; i<=bnib; i++) {
      b1=b[i] % bi;
      q[k]=(double)(b1)*isize;
      b1=b[i] / bi;
      q[k+2]=(double)(b1)*isize;
      k=k+4;
   } /* for i */

   k=0;

   for (i=ib; i<=cnib; i++) {
      c1=c[i] % bi;
      q[k+1]=(double)(c1)*isize;
      c1=c[i] / bi;
      q[k+3]=(double)(c1)*isize;
      k=k+4;
   } /* for i */


   cdft(n2,1,q);

   mul(q,n);

   cdft(n2,-1,q);

   maxerror=0.0;

   rr=0.0;

   for (i=0; i<=n2; i=i+2) {
      qqq=q[i];
      temp=fabs(qqq-floor(qqq+0.5));
      if (temp > maxerror) {
         maxerror=temp;
      } /* if */
      xx=qqq+rr;
      rr=floor((xx+0.5)*bri);
      q[i]=xx-br*rr; 
   } /* for i */

   if (maxerror > fftmaxerror) {
      fftmaxerror=maxerror;
   } /* if */

   k=4*dl;

   for (i=ib; i<=l; i++) {
      a[i]=0;
      if (k <= n2) {
         a[i]=bi*(int)(q[k+2]+0.5)+(int)(q[k]+0.5);
      } /* if */
      k=k+4;
   } /* for i */

   free(q);

   }
   else {

   for (i=ib; i<=l; i++) {
      a[i]=0;
   } /* for i */

   } /* if */

   a[l+1]=b[l+1]*c[l+1]; 

} /* if */


} /* lmul */


void linv(a,b,nn)
short *a;
short *b;
int *nn;
{

   double man,ex;
   
   int s,l,il,dl,dd,kk,bb,bbb,i;

   short *x1,*x2,*t1,*t2;

   int ss[4];
 
   l=nn[0]; il=nn[1]; dl=nn[2];
   dd=il+dl;

   x1=(short *)malloc((l+2)*sizeof(short));
   x2=(short *)malloc((l+2)*sizeof(short));
   t1=(short *)malloc((l+2)*sizeof(short));
   t2=(short *)malloc((l+2)*sizeof(short));

   lzero(x1,nn);
   lzero(x2,nn);
   lzero(t1,nn);
   lzero(t2,nn);
 
   s=b[l+1];
   b[l+1]=1;

   ltoreal(b,&man,&ex,nn);

   man=1.0/man;
   ex=-ex;

   lrealtol(x1,man,ex,nn);

   bbb=lcheckdec(b,nn);

   if (bbb < 16) {
      bb=32;
   }
   else {
      kk=(int)(log((double)(bbb+2))/log(2.0));
      bb=2;
      for (i=1; i<=kk; i++) {
         bb=2*bb;
      } /* for i */
   } /* if */

   kk=bb;

   ss[0]=l; ss[1]=il;

   do {
      if (kk > dd) {
         kk=dd;
      } /* if */
      ss[2]=kk-il;

      lmul(t1,x1,b,ss);
      linttol(t2,1,0,ss);
      lsub(x2,t2,t1,ss);

      lmul(t1,x2,x2,ss);
      lmul(a,x2,t1,ss);
      ladd(t2,t1,a,ss);
      ladd(t1,x2,t2,ss);
      lmul(t2,t1,x1,ss);
      ladd(t1,x1,t2,ss);
      lass(x1,t1,ss);

      kk=4*kk;
   } while (lcompeps(x2,nn)==1); 

   lass(a,x1,nn);

   a[l+1]=s;

   free(x1);
   free(x2);
   free(t1);
   free(t2);

} /* linv */ 
 
 
void ldivv(a,b,c,nn)
short *a;
short *b;
short *c;
int *nn;
{

   linv(a,c,nn);

   lmul(a,a,b,nn);

} /* ldivv */    

 
void lsqrtinv(a,b,nn)
short *a;
short *b;
int *nn;
{

   double re0,re1,re2,s10,man,ex,eps;

   int s,l,il,dl,dd,kk,bb,bbb,i;

   short *x1,*x2,*t1,*t2;
    
   int ss[4]; 
 
   l=nn[0]; il=nn[1]; dl=nn[2];
   dd=il+dl;

   x1=(short *)malloc((l+2)*sizeof(short));
   x2=(short *)malloc((l+2)*sizeof(short));
   t1=(short *)malloc((l+2)*sizeof(short));
   t2=(short *)malloc((l+2)*sizeof(short));

   lzero(x1,nn);
   lzero(x2,nn);
   lzero(t1,nn);
   lzero(t2,nn);

   eps=0.001; 
   s=b[l+1];
   b[l+1]=1;

   ltoreal(b,&man,&ex,nn);

   re0=1.0/sqrt(man);
   re1=-ex/2.0;
   re2=fabs(re1);

   s10=sqrt(10.0);

   if ((re2-(double)((int)(re2)))>eps) {
      re0=re0/s10;
      re1=re1+0.5;
   } /* if */ 

   man=re0;
   ex=re1;

   lrealtol(x1,man,ex,nn);

   bbb=lcheckdec(b,nn);

   if (bbb < 16) {
      bb=32;
   }
   else {
      kk=(int)(log((double)(bbb+2))/log(2.0));
      bb=2;
      for (i=1; i<=kk; i++) {
         bb=2*bb;
      } /* for i */
   } /* if */

   kk=bb;

   ss[0]=l; ss[1]=il; 

   do {
      if (kk > dd) {
         kk=dd;
      } /* if */ 
      ss[2]=kk-il;

      lmul(t1,x1,b,ss);
      lmul(t1,x1,t1,ss);
      linttol(t2,1,0,ss);
      lsub(x2,t2,t1,ss);

      lmul(t1,x2,x2,ss);
      lmul(a,t1,x2,ss);
      lcmul(t2,a,5,0,ss);
      lcmul(a,t1,6,0,ss);
      ladd(t1,a,t2,ss);
      lcmul(t2,x2,8,0,ss);
      ladd(a,t1,t2,ss);
      lmul(t1,a,x1,ss);
      lcmul(a,t1,625,-1,ss);
      ladd(t2,x1,a,ss);
      lass(x1,t2,ss);

      kk=4*kk;
   } while (lcompeps(x2,nn)==1); 

   lass(a,x1,nn);

   free(x1);
   free(x2);
   free(t1);
   free(t2);

} /* lsqrtinv */


void lsqrt(a,b,nn)
short *a;
short *b;
int *nn;
{

   lsqrtinv(a,b,nn);

   lmul(a,a,b,nn);

} /* lsqrt */


void lsqrtsqrtinv(a,b,nn)
short *a;
short *b;
int *nn;
{

   double re0,re1,re2,s10,man,ex,eps;

   int s,l,il,dl,bb,dd,kk,bbb,i;

   short *x1,*x2,*t1,*t2;

   int ss[4];
 
   l=nn[0]; il=nn[1]; dl=nn[2];
   dd=il+dl;

   x1=(short *)malloc((l+2)*sizeof(short));
   x2=(short *)malloc((l+2)*sizeof(short));
   t1=(short *)malloc((l+2)*sizeof(short));
   t2=(short *)malloc((l+2)*sizeof(short));

   lzero(x1,nn);
   lzero(x2,nn);
   lzero(t1,nn);
   lzero(t2,nn);
 
   eps=0.001;
   s=b[l+1];
   b[l+1]=1;

   ltoreal(b,&man,&ex,nn);

   re0=1.0/sqrt(sqrt(man));
   re1=-ex/4.0;
   re2=fabs(re1);

   s10=sqrt(sqrt(10.0));

   while ((re2-(double)((int)(re2)))>eps) {
      re0=re0/s10;
      re1=re1+0.25;
      re2=fabs(re1);
   } /* while */

   man=re0;
   ex=re1;

   lrealtol(x1,man,ex,nn);

   bbb=lcheckdec(b,nn);

   if (bbb < 16) {
      bb=32;
   }
   else {
      kk=(int)(log((double)(bbb+2))/log(2.0));
      bb=2;
      for (i=1; i<=kk; i++) {
         bb=2*bb;
      } /* for i */
   } /* if */

   kk=bb;

   ss[0]=l; ss[1]=il; 

   do {
      if (kk > dd) {
         kk=dd;
      } /* if */
      ss[2]=kk-il;

      lmul(t1,x1,x1,ss);
      lmul(t1,t1,t1,ss);
      lmul(t1,t1,b,ss);
      linttol(t2,1,0,ss);
      lsub(x2,t2,t1,ss);

      lmul(t1,x2,x2,ss);
      lmul(a,t1,x2,ss);
      lcmul(t2,a,15,0,ss);
      lcmul(a,t1,20,0,ss);
      ladd(t1,a,t2,ss);
      lcmul(t2,x2,32,0,ss);
      ladd(a,t1,t2,ss);
      lmul(t1,a,x1,ss);
      lcmul(a,t1,781250,-2,ss);
      ladd(t2,x1,a,ss);
      lass(x1,t2,ss);

      kk=4*kk;
   } while (lcompeps(x2,nn)==1); 

   lass(a,x1,nn);

   free(x1);
   free(x2);
   free(t1);
   free(t2);

} /* lsqrtsqrtinv */


void lsqrtsqrt(a,b,nn)
short *a;
short *b;
int *nn;
{

  int l,il,dl,dd;

   short *t1;

   l=nn[0]; il=nn[1]; dl=nn[2];
   dd=il+dl;

   lsqrtsqrtinv(a,b,nn);

   t1=(short *)malloc((l+2)*sizeof(short));

   lmul(t1,a,a,nn);
   lmul(t1,t1,a,nn);
   lmul(a,t1,b,nn);

   free(t1);

} /* lsqrtsqrt */


void linv3(a,nn)
short *a;
int *nn;
{
   int i,l,il,dl,ib,ll;

   l=nn[0]; il=nn[1]; dl=nn[2];

   ll=l-il+1;
   ib=l-il-dl+1;

   for (i=l;i>=ll; i--) {
      a[i]=0;
   } /* for i */

   for (i=ll-1;i>=ib; i--) {
      a[i]=3333;
   } /* for i */

   a[l+1]=1;

} /* linv3 */


void linv162(a,nn)
short *a;
int *nn;
{
   int i,l,il,dl,ib,ll,kk;

   char cc[12];
   char ss[12];

   cc[0]='7';
   cc[1]='2';
   cc[2]='8';
   cc[3]='3';
   cc[4]='9';
   cc[5]='5';
   cc[6]='0';
   cc[7]='6';
   cc[8]='1';
   cc[9]='\0';

   l=nn[0]; il=nn[1]; dl=nn[2];

   ll=l-il+1;
   ib=l-il-dl+1;

   for (i=l;i>=ll; i--) {
      a[i]=0;
   } /* for i */

   a[ll-1]=61;

   kk=0;

   for (i=ll-2;i>=ib; i--) {

      ss[0]=cc[(kk % 9)];
      ss[1]=cc[((kk+1) % 9)];
      ss[2]=cc[((kk+2) % 9)];
      ss[3]=cc[((kk+3) % 9)];
      ss[4]='\0';

      a[i]=atoi(ss);

      kk=kk+4;

   } /* for i */

   a[l+1]=1;

} /* linv162 */


void lcubertinv(a,b,nn)
short *a;
short *b;
int *nn;
{

   double re0,re1,re2,s10,man,ex,eps;

   int s,l,il,dl,bb,dd,kk,bbb,i;

   short *x1,*x2,*t1,*t2,*t3;

   int ss[4];
 
   l=nn[0]; il=nn[1]; dl=nn[2];
   dd=il+dl;

   x1=(short *)malloc((l+2)*sizeof(short));
   x2=(short *)malloc((l+2)*sizeof(short));
   t1=(short *)malloc((l+2)*sizeof(short));
   t2=(short *)malloc((l+2)*sizeof(short));
   t3=(short *)malloc((l+2)*sizeof(short));

   lzero(x1,nn);
   lzero(x2,nn);
   lzero(t1,nn);
   lzero(t2,nn);

   linv162(t3,nn);

   eps=0.001;
   s=b[l+1];
   b[l+1]=1;

   ltoreal(b,&man,&ex,nn);

   re0=1.0/exp(log(man)/3.0);
   re1=-ex/3.0;
   re2=fabs(re1);

   s10=exp(log(10.0)/3.0);

   while ((re2-(double)((int)(re2)))>eps) {
      re0=re0/s10;
      re1=re1+1.0/3.0;
      re2=fabs(re1);
   } /* while */

   man=re0;
   ex=re1;

   lrealtol(x1,man,ex,nn);

   bbb=lcheckdec(b,nn);

   if (bbb < 16) {
      bb=32;
   }
   else {
      kk=(int)(log((double)(bbb+2))/log(2.0));
      bb=2;
      for (i=1; i<=kk; i++) {
         bb=2*bb;
      } /* for i */
   } /* if */

   kk=bb;

   ss[0]=l; ss[1]=il; 

   do {
      if (kk > dd) {
         kk=dd;
      } /* if */
      ss[2]=kk-il;

      lmul(t1,x1,x1,ss);
      lmul(t1,t1,x1,ss);
      lmul(t1,t1,b,ss);
      linttol(t2,1,0,ss);
      lsub(x2,t2,t1,ss);

      lmul(t1,x2,x2,ss);
      lmul(a,t1,x2,ss);
      lcmul(t2,a,28,0,ss);
      lcmul(a,t1,36,0,ss);
      ladd(t1,a,t2,ss);
      lcmul(t2,x2,54,0,ss);
      ladd(a,t1,t2,ss);
      lmul(t1,a,x1,ss);
      lmul(a,t3,t1,ss);
      ladd(t2,x1,a,ss);
      lass(x1,t2,ss);

      kk=4*kk;
   } while (lcompeps(x2,nn)==1); 

   lass(a,x1,nn);

   free(x1);
   free(x2);
   free(t1);
   free(t2);
   free(t3);

} /* lcubertinv */


void lcubert(a,b,nn)
short *a;
short *b;
int *nn;
{

  int l,il,dl,dd;

   short *t1;

   l=nn[0]; il=nn[1]; dl=nn[2];
   dd=il+dl;

   lcubertinv(a,b,nn);

   t1=(short *)malloc((l+2)*sizeof(short));

   lmul(t1,a,a,nn);
   lmul(a,t1,b,nn);

   free(t1);

} /* lcubert */


void biginit(a,maxsize)
bigint *a;
int maxsize;
{
   a->coef=(short *)malloc((maxsize+1)*sizeof(short));
   a->size=1;
   a->maxsize=maxsize;
   a->exp0=0;
   a->sgn0=1;

   a->coef[1]=0;
} /* biginit */


void biginit2(a,maxsize)
bigint *a;
int maxsize;
{
   int i;

   a->coef=(short *)malloc((maxsize+1)*sizeof(short));
   a->size=1;
   a->maxsize=maxsize;
   a->exp0=0;
   a->sgn0=1;

   for (i=1; i<=maxsize; i++) {
      a->coef[i]=0;
   } /* for i */

} /* biginit2 */


void bigfree(a)
bigint *a;
{
  free(a->coef);
} /* bigfree */


void inttobig(a,bb)
bigint *a;
int bb;
{
   int tt1,tt2,t1,t2,b;

   b=abs(bb);

   if (b < 10000) {
      a->coef[1]=b;
      a->size=1;
   }
   else {
      tt1=b % 100000000;
      tt2=b / 100000000;
      t1=tt1 % 10000;
      t2=tt1 / 10000;
      a->coef[1]=t1;
      a->coef[2]=t2;
      a->coef[3]=tt2;
      a->size=1;
      if (t2 > 0) {
         a->size=2;
      } /* if */
      if (tt2 > 0) {
         a->size=3;
      } /* if */
   } /* if */

   a->exp0=0;

   if (bb >= 0) {
      a->sgn0=1;
   }
   else {
      a->sgn0=-1;
   } /* if */

} /* inttobig */


void bigass(a,b)
bigint *a;
bigint *b;
{
   int i,n;

   n=b->size;

   for (i=1; i<=n; i++) {
      a->coef[i]=b->coef[i];
   } /* for i */

   a->size=n;
   a->exp0=b->exp0;
   a->sgn0=b->sgn0;

} /* bigass */


int bigcomp2(b,c)
bigint *b;
bigint *c;
{

  int i,n,bn,cn,bb,cc,maxsize,maxs;
  int bexp,cexp,expmax;
  int bk,ck,bi,ci,flag;

  bn=b->size;
  cn=c->size;

  maxsize=b->maxsize;
  maxs=maxsize-2;

  bexp=b->exp0;
  cexp=c->exp0;

  if ((bn+bexp) > (cn+cexp)) {
     expmax=bexp;
  }
  else {
     expmax=cexp;
  } /* if */

  if (bn > cn) {
     n=bn;
  }
  else {
     n=cn;
  } /* if */

  bi=expmax-bexp;
  ci=expmax-cexp;

  i=n;
  flag=0;

  while ((flag == 0) && (i >= 1)) {

     bk=i+bi;
     if ((bk <= bn) && (bk >= 1)) {
        bb=b->coef[bk];
     }
     else {
        bb=0;
     } /* if */

     ck=i+ci;
     if ((ck <= cn) && (ck >= 1)) {
        cc=c->coef[ck];
     } 
     else {
        cc=0;
     } /* if */

     if (bb != cc) {
        flag=1;
     } /* if */

     i--;

  } /* while*/

  if (bb >= cc) {
     return(1);
  }
  else {
     return(-1);
  } /* if */


} /* bigcomp2 */
      

void bigadd2(a,b,c)
bigint *a;
bigint *b;
bigint *c;
{

  int i,n,bn,cn,bb,cc,maxsize,maxs;
  int q,r,basint,bexp,cexp,expmax;
  int bk,ck,bi,ci;

  bn=b->size;
  cn=c->size;

  maxsize=b->maxsize;
  maxs=maxsize-2;

  bexp=b->exp0;
  cexp=c->exp0;

  if ((bn+bexp) > (cn+cexp)) {
     expmax=bexp;
  }
  else {
     expmax=cexp;
  } /* if */

  if (bn > cn) {
     n=bn;
  }
  else {
     n=cn;
  } /* if */

  bi=expmax-bexp;
  ci=expmax-cexp;

  basint=10000;
  r=0;

  for (i=1; i<=n; i++) {
     bk=i+bi;
     if ((bk <= bn) && (bk >= 1)) {
        bb=b->coef[bk];
     }
     else {
        bb=0;
     } /* if */

     ck=i+ci;
     if ((ck <= cn) && (ck >= 1)) {
        cc=c->coef[ck];
     } 
     else {
        cc=0;
     } /* if */

     q=bb+cc+r;
     if (q >= basint) {
        a->coef[i]=q-basint;
	r=1;
     }
     else {
        a->coef[i]=q;
	r=0;
     } /* if */
  } /* for i */


  if (n >= maxs) {

     if (r > 0) {

        for (i=1; i < n; i++) {
           a->coef[i]=a->coef[i+1];
        } /* for i */

        a->coef[n]=r;
        a->size=n;
        a->exp0=expmax+1;
     }
     else {
        a->size=n;
        a->exp0=expmax;
     } /* if */

  }
  else {
  
     if (r > 0) {
        a->coef[n+1]=r;
        a->size=n+1;
        a->exp0=expmax;
     }
     else {
        a->size=n;
        a->exp0=expmax;
     } /* if */

  } /* if */

} /* bigadd2 */
      

void bigsub2(a,b,c)
bigint *a;
bigint *b;
bigint *c;
{

  int i,n,bn,cn,bb,cc,maxsize,maxs;
  int q,r,basint,bexp,cexp,expmax;
  int bk,ck,bi,ci;

  bn=b->size;
  cn=c->size;

  maxsize=b->maxsize;
  maxs=maxsize-2;

  bexp=b->exp0;
  cexp=c->exp0;

  if ((bn+bexp) > (cn+cexp)) {
     expmax=bexp;
  }
  else {
     expmax=cexp;
  } /* if */

  if (bn > cn) {
     n=bn;
  }
  else {
     n=cn;
  } /* if */

  bi=expmax-bexp;
  ci=expmax-cexp;

  basint=10000;
  r=0;

  for (i=1; i<=n; i++) {
     bk=i+bi;
     if ((bk <= bn) && (bk >= 1)) {
        bb=b->coef[bk];
     }
     else {
        bb=0;
     } /* if */

     ck=i+ci;
     if ((ck <= cn) && (ck >= 1)) {
        cc=c->coef[ck];
     } 
     else {
        cc=0;
     } /* if */

     q=bb-cc-r;
     if (q < 0) {
        a->coef[i]=basint+q;
	r=1;
     }
     else {
        a->coef[i]=q;
	r=0;
     } /* if */

  } /* for i */

  i=n;

  while ((a->coef[i] == 0) && (i > 1)) {
     i--;
  } /* while */

  a->size=i;
  a->exp0=expmax;

} /* bigsub2 */


void bigadd(a,b,c)
bigint *a;
bigint *b;
bigint *c;
{
   int b2,c2,co;

   co=bigcomp2(b,c);
  
   b2=b->sgn0;
   c2=c->sgn0; 

      if ((b2>0) && (c2>0)) {
         bigadd2(a,b,c);
	 a->sgn0=1;
      } /* if */
      if ((b2>0) && (c2<0) && (co==1)) {
         bigsub2(a,b,c);
	 a->sgn0=1;
      } /* if */
      if ((b2>0) && (c2<0) && (co==-1)) {
         bigsub2(a,c,b);
         a->sgn0=-1;
      } /* if */
      if ((b2<0) && (c2>0) && (co==1)) {
         bigsub2(a,b,c);
         a->sgn0=-1;
      } /* if */
      if ((b2<0) && (c2>0) && (co==-1)) {
         bigsub2(a,c,b);
	 a->sgn0=1;
      } /* if */
      if ((b2<0) && (c2<0)) {
         bigadd2(a,b,c);
	 a->sgn0=-1;
      } /* if */

} /* bigadd */


void bigsub(a,b,c,nn)
bigint *a;
bigint *b;
bigint *c;
int *nn;
{
   int b2,c2,co;

   co=bigcomp2(b,c);
  
   b2=b->sgn0;
   c2=c->sgn0; 

      if ((b2>0) && (c2>0) && (co==1)) {
         bigsub2(a,b,c);
         a->sgn0=1;
      } /* if */
      if ((b2>0) && (c2>0) && (co==-1)) {
         bigsub2(a,c,b);
         a->sgn0=-1;
      } /* if */
      if ((b2>0) && (c2<0)) {
         bigadd2(a,b,c);
         a->sgn0=1;
      } /* if */
      if ((b2<0) && (c2>0)) {
         bigadd2(a,b,c);
         a->sgn0=-1;
      } /* if */
      if ((b2<0) && (c2<0) && (co==1)) {
         bigsub2(a,c,b);
         a->sgn0=-1;
      } /* if */
      if ((b2<0) && (c2<0) && (co==-1)) {
         bigsub2(a,b,c);
         a->sgn0=1;
      } /* if */

} /* bigsub */


void bigmul(a,b,c)
bigint *a;
bigint *b;
bigint *c;
{

   double *q;

   int i,k,n,n2;
   int bi,b1,c1;
   int bn,cn,kkk,maxsize;
   int bexp,cexp,exptot,gg,maxs;

   double isize,rr,xx;
   double br,bri,maxerror,qqq,temp;


if (mulversion == 1) {

   bn=b->size;
   cn=c->size;

   bexp=b->exp0;
   cexp=c->exp0;
   exptot=bexp+cexp;

   maxsize=b->maxsize;
   maxs=maxsize-2;

   kkk=bn+cn;

   n=1;

   while (n < kkk) {
      n=2*n;
   } /* while */

   n2=2*n;

   q=(double *)malloc((n2+1)*sizeof(double));

   for (i=0; i<=n2; i++) {
      q[i]=0.0;
   } /* for i */

   isize=1.0/sqrt((double)(n));
 
   k=0;

   for (i=1; i<=bn; i++) {
      q[k]=(double)(b->coef[i])*isize;
      k=k+2;
   } /* for i */

   k=0;

   for (i=1; i<=cn; i++) {
      q[k+1]=(double)(c->coef[i])*isize;
      k=k+2;
   } /* for i */

   cdft(n2,1,q);

   mul(q,n);

   cdft(n2,-1,q);

   maxerror=0.0;

   rr=0.0;

   for (i=0; i<=n2; i=i+2) {
      qqq=q[i];
      temp=fabs(qqq-floor(qqq+0.5));
      if (temp > maxerror) {
         maxerror=temp;
      } /* if */
      xx=qqq+rr;
      rr=floor((xx+0.5)*basi);
      q[i]=xx-bas*rr; 
   } /* for i */

   if (maxerror > fftmaxerror) {
      fftmaxerror=maxerror;
   } /* if */

   gg=1;
   if (kkk > maxs) {
   /*
      printf("* %8d %8d\n",kkk,maxs);
      */
      gg=kkk-maxs+1;
   } /* if */

   k=2*gg-2;

   for (i=gg; i<=kkk; i++) {
      a->coef[i-gg+1]=(int)(q[k]+0.5);
      k=k+2;
   } /* for i */


   if (a->coef[kkk-gg+1] > 0) {
      a->size=kkk-gg+1;
      a->exp0=exptot+gg-1;
   }
   else {
      a->size=kkk-gg;
      a->exp0=exptot+gg-1;
   } /* if */

   a->sgn0=(b->sgn0)*(c->sgn0);


   free(q);


} 
else {

   br=100.0;
   bri=0.01;
   bi=100;

   bn=b->size;
   cn=c->size;

   bexp=b->exp0;
   cexp=c->exp0;
   exptot=bexp+cexp;

   maxsize=b->maxsize;
   maxs=maxsize-2;

   kkk=bn+cn;

   n=1;

   while (n < kkk) {
      n=2*n;
   } /* while */

   n=2*n;
   n2=2*n;

   q=(double *)malloc((n2+1)*sizeof(double));

   for (i=0; i<=n2; i++) {
      q[i]=0.0;
   } /* for i */

   isize=1.0/sqrt((double)(n));
 
   k=0;

   for (i=1; i<=bn; i++) {
      b1=b->coef[i] % bi;
      q[k]=(double)(b1)*isize;
      b1=b->coef[i] / bi;
      q[k+2]=(double)(b1)*isize;
      k=k+4;
   } /* for i */

   k=0;

   for (i=1; i<=cn; i++) {
      c1=c->coef[i] % bi;
      q[k+1]=(double)(c1)*isize;
      c1=c->coef[i] / bi;
      q[k+3]=(double)(c1)*isize;
      k=k+4;
   } /* for i */

   cdft(n2,1,q);

   mul(q,n);

   cdft(n2,-1,q);

   maxerror=0.0;

   rr=0.0;

   for (i=0; i<=n2; i=i+2) {
      qqq=q[i];
      temp=fabs(qqq-floor(qqq+0.5));
      if (temp > maxerror) {
         maxerror=temp;
      } /* if */
      xx=qqq+rr;
      rr=floor((xx+0.5)*bri);
      q[i]=xx-br*rr; 
   } /* for i */

   if (maxerror > fftmaxerror) {
      fftmaxerror=maxerror;
   } /* if */

   gg=1;
   if (kkk > maxs) {
   /*
      printf("* %8d %8d\n",kkk,maxs);
      */
      gg=kkk-maxs+1;
   } /* if */

   k=4*gg-4;

   for (i=gg; i<=kkk; i++) {
      a->coef[i-gg+1]=bi*(int)(q[k+2]+0.5)+(int)(q[k]+0.5);
      k=k+4;
   } /* for i */

   if (a->coef[kkk-gg+1] > 0) {
      a->size=kkk-gg+1;
      a->exp0=exptot+gg-1;
   }
   else {
      a->size=kkk-gg;
      a->exp0=exptot+gg-1;
   } /* if */

   a->sgn0=(b->sgn0)*(c->sgn0);

   free(q);

} /* if */

} /* bigmul */


void big2l(a_big,b_big,a_l,b_l,nn)
bigint *a_big;
bigint *b_big;
short *a_l;
short *b_l;
int *nn;
{
   int i,ii,l,il,dl,ib,k,kk,jj,maxexp;

   l=nn[0]; il=nn[1]; dl=nn[2];

   ib=l-il-dl+1;

   if (b_big->exp0 > a_big->exp0) {
      maxexp=b_big->exp0;
   } 
   else {
      maxexp=a_big->exp0;
   } /* if */

   if (b_big->size > a_big->size) {
      ii=b_big->size;
   }
   else {
      ii=a_big->size;
   } /* if */

   k=l-il;

   for (i=ii; i >= 1; i--) {
      if (k >= 1) {
         jj=i+maxexp-b_big->exp0;
         if (jj <= b_big->size) {
            b_l[k]=b_big->coef[jj];
	 } /* if */
         kk=i+maxexp-a_big->exp0;
         if (kk <= a_big->size) {
            a_l[k]=a_big->coef[kk];
	 } /* if */
      } /* if */
      k--;
   } /* for i */

} /* big2l */


double julian_day(year,month,day)
int year,month,day;
{
   double f,g,a,jdd;

   if (month>=3) {
      f=(double)(year);
      g=(double)(month);
   }
   else {
      f=(double)(year-1);
      g=(double)(month+12);
   } /* if */

   a=2.0-floor(f/100.0)+floor(f/400.0);

   jdd=floor(365.25*f)+floor(30.6001*(g+1.0))+
       (double)(day)+a+1720994.5;

   return(jdd);

} /* julian_day */


double get_float_days(ttime)
time_t *ttime;
{
   int year,month,day,hour,min,sec;

   double fdays,fhours;

   struct tm *tt;

   tt=localtime(ttime);

   year=tt->tm_year+1900;
   month=tt->tm_mon+1;
   day=tt->tm_mday;

   hour=tt->tm_hour;
   min=tt->tm_min;
   sec=tt->tm_sec;
  
   fhours=(double)(hour)+(double)(min)/60.0+(double)(sec)/3600.0;

   fdays=julian_day(year,month,day)+fhours/24.0;

   return(fdays);

} /* get_float_days */


void day_and_time(ttime)
time_t *ttime;
{

   int year,month,day,hour,min,sec;

   struct tm *tt;

   tt=localtime(ttime);

   year=tt->tm_year+1900;
   month=tt->tm_mon+1;
   day=tt->tm_mday;

   hour=tt->tm_hour;
   min=tt->tm_min;
   sec=tt->tm_sec;

   printf("%04d-%02d-%02d %02d:%02d:%02d",year,month,day,hour,min,sec);

} /* day_and_time */


void fday_and_time(outfile,ttime)
FILE *outfile;
time_t *ttime;
{

   int year,month,day,hour,min,sec;

   struct tm *tt;

   tt=localtime(ttime);

   year=tt->tm_year+1900;
   month=tt->tm_mon+1;
   day=tt->tm_mday;

   hour=tt->tm_hour;
   min=tt->tm_min;
   sec=tt->tm_sec;

   fprintf(outfile,"%04d-%02d-%02d %02d:%02d:%02d",year,month,day,hour,min,sec);

} /* fday_and_time */


void sum_zeta3_1(rP,rQ,rT,n1,n2,maxsize)
bigint *rP;
bigint *rQ;
bigint *rT;
int n1;
int n2;
int maxsize;
{

   int nm,n;

   bigint LP,LQ,LT;
   bigint RP,RQ,RT;

   if ((n2-n1) == 1) {

      n=n1;

      if (n == 0) {

         inttobig(rP,1);
         inttobig(rQ,1);
         inttobig(rT,77);

      } 
      else {


         inttobig(&tmp,(n));
         inttobig(&tmp2,(n));
	 bigmul(&tmp,&tmp,&tmp2);

	 bigmul(&tmp3,&tmp,&tmp);
	 bigmul(&tmp3,&tmp3,&tmp3);
	 bigmul(rP,&tmp3,&tmp);
	 rP->sgn0=-1;

       
         inttobig(&tmp2,(n));
	 inttobig(&tmp3,(250));
	 bigmul(&tmp3,&tmp3,&tmp2);
         inttobig(&tmp2,(77));
	 bigadd(&tmp4,&tmp3,&tmp2);
	 inttobig(&tmp3,(205));
	 bigmul(&tmp,&tmp3,&tmp);
         bigadd(&tmp3,&tmp,&tmp4);

	 bigmul(rT,rP,&tmp3);

	 inttobig(&tmp,(2*n+1));
         inttobig(&tmp2,(2*n));
	 bigmul(&tmp,&tmp2,&tmp);
	 bigmul(&tmp2,&tmp,&tmp);
	 bigmul(&tmp2,&tmp2,&tmp2);
	 bigmul(rQ,&tmp2,&tmp);

     } /* if */


   }
   else {

      biginit(&LP,maxsize);
      biginit(&LQ,maxsize);
      biginit(&LT,maxsize);

      biginit(&RP,maxsize);
      biginit(&RQ,maxsize);
      biginit(&RT,maxsize);

      nm=(n1+n2)/2;

      sum_zeta3_1(&LP,&LQ,&LT,n1,nm,maxsize);

      sum_zeta3_1(&RP,&RQ,&RT,nm,n2,maxsize);


      bigmul(rP,&LP,&RP);
      bigmul(rQ,&LQ,&RQ);

      bigmul(&tmp,&RQ,&LT);

      bigmul(&tmp2,&LP,&RT);

      bigadd(rT,&tmp,&tmp2);

      bigfree(&LP);
      bigfree(&LQ);
      bigfree(&LT);
      
      bigfree(&RP);
      bigfree(&RQ);
      bigfree(&RT);


   } /* if */

} /* sum_zeta3_1 */


void a_factor(a,n,maxsize)
bigint *a;
int n;
int maxsize;
{

   inttobig(&tmp4,(12463));

   inttobig(&tmp,(n));

   inttobig(&tmp5,(n));
   inttobig(&tmp2,(104000));
   bigmul(&tmp3,&tmp2,&tmp5);
   bigadd(a,&tmp4,&tmp3);

   bigmul(&tmp5,&tmp5,&tmp);
   inttobig(&tmp2,(336367));
   bigmul(&tmp3,&tmp2,&tmp5);
   bigadd(&tmp4,a,&tmp3);

   bigmul(&tmp5,&tmp5,&tmp);
   inttobig(&tmp2,(531578));
   bigmul(&tmp3,&tmp2,&tmp5);
   bigadd(a,&tmp4,&tmp3);

   bigmul(&tmp5,&tmp5,&tmp);
   inttobig(&tmp2,(412708));
   bigmul(&tmp3,&tmp2,&tmp5);
   bigadd(&tmp4,a,&tmp3);

   bigmul(&tmp5,&tmp5,&tmp);
   inttobig(&tmp2,(126392));
   bigmul(&tmp3,&tmp2,&tmp5);
   bigadd(a,&tmp4,&tmp3);

} /* a_factor */


void b_factor(bp,bq,n,maxsize)
bigint *bp;
bigint *bq;
int n;
int maxsize;
{

   inttobig(&tmp,(2*n+1));
   inttobig(&tmp2,(2*n));
   bigmul(&tmp,&tmp,&tmp2);
   bigmul(&tmp,&tmp,&tmp2);
   inttobig(&tmp2,(2*n-1));
   bigmul(&tmp,&tmp,&tmp2);
   inttobig(&tmp2,(n));
   bigmul(&tmp,&tmp,&tmp2);

   bigmul(bp,&tmp,&tmp);
   bigmul(bp,bp,&tmp);
   
   inttobig(&tmp,(3*n+2));
   inttobig(&tmp2,(3*n+1));
   bigmul(&tmp,&tmp,&tmp2);
   inttobig(&tmp2,(3*n));
   bigmul(bq,&tmp,&tmp2);

   inttobig(&tmp,(4*n+3));
   inttobig(&tmp2,(4*n+2));
   bigmul(&tmp,&tmp,&tmp2);
   inttobig(&tmp2,(4*n+1));
   bigmul(&tmp,&tmp,&tmp2);
   inttobig(&tmp2,(4*n));
   bigmul(&tmp,&tmp,&tmp2);

   bigmul(&tmp2,&tmp,&tmp);
   bigmul(&tmp2,&tmp2,&tmp);

   bigmul(bq,bq,&tmp2);

} /* b_factor */


void sum_zeta3_2(rP,rQ,rT,n1,n2,maxsize)
bigint *rP;
bigint *rQ;
bigint *rT;
int n1;
int n2;
int maxsize;
{

   int nm,n;

   bigint LP,LQ,LT;
   bigint RP,RQ,RT;

   if ((n2-n1) == 1) {

      n=n1;

      if (n == 0) {

         inttobig(rP,1);
         inttobig(rQ,432);
         inttobig(rT,12463);

      } 
      else {

         b_factor(rP,rQ,n,maxsize);
	 rP->sgn0=-1;

         a_factor(rT,n,maxsize);

	 bigmul(rT,rT,rP);

     } /* if */


   }
   else {

      biginit(&LP,maxsize);
      biginit(&LQ,maxsize);
      biginit(&LT,maxsize);

      biginit(&RP,maxsize);
      biginit(&RQ,maxsize);
      biginit(&RT,maxsize);

      nm=(n1+n2)/2;

      sum_zeta3_2(&LP,&LQ,&LT,n1,nm,maxsize);

      sum_zeta3_2(&RP,&RQ,&RT,nm,n2,maxsize);


      bigmul(rP,&LP,&RP);
      bigmul(rQ,&LQ,&RQ);

      bigmul(&tmp,&RQ,&LT);

      bigmul(&tmp2,&LP,&RT);

      bigadd(rT,&tmp,&tmp2);

      bigfree(&LP);
      bigfree(&LQ);
      bigfree(&LT);
      
      bigfree(&RP);
      bigfree(&RQ);
      bigfree(&RT);


   } /* if */

} /* sum_zeta3_2 */


int main(argc,argv) 
int argc;
char *argv[];
{

   FILE *outfile,*mvfile;

   int maxmem,bigsize,lsize;
   int i,n,length,ilength,pot,pot0;
   int mem1,mem2,decimals,decimals2;
   int n1,n2,method;

   double elapseddays,frac,temp;

   int nn[5];

   clock_t time1,timediff1;
   clock_t time2,timediff2;

   time_t start_time,stop_time;

   short *a,*b,*zeta3_result; 

   bigint rP;
   bigint rQ;
   bigint rT;


   if ((mvfile = fopen(mv_file_name,"r")) == NULL) {
      printf("\n");
      printf("Warning, file %s not found, mulversion now set to 1!\n",mv_file_name);
      printf("\n");
      mulversion=1;
   } 
   else {
      fscanf(mvfile,"%d",&mulversion);
      fclose(mvfile);

      if ((mulversion != 1) && (mulversion != 2)) {
         printf("\n");
         printf("Warning, wrong value of mulversion in %s, mulversion now set to 1!\n",mv_file_name);
         printf("\n");
	 mulversion=1;
      } /* if */

   } /* if */


   if (argc != 3) {
      printf("\n");
      printf("Usage: %s methodi power-of-2i\n",argv[0]);
      printf("\n");
      printf("%s\n",version_string);
      printf("\n");
      printf("method = 1 => Apery's Zeta(3) using fast series 1.\n");
      printf("method = 2 => Apery's Zeta(3) using fast series 2.\n");
      printf("\n");
      printf("Zeta(3) is calculated with about 2^(power-of-2) decimal digits.\n");
      printf("\n");
      printf("The variable mulversion is at present set to %1d.\n",mulversion);
      printf("It is possible to change the value (to %1d) by editing the file \"%s\".\n",
             3-mulversion,mv_file_name);
      printf("Set the variable to 2 for calculations with more than 2^24 (16 million) digits,\n");
      printf("or more important at least when \"FFT max error\" > 0.25.\n");
      printf("\n");
      exit(0);
   } /* if */

   method=atoi(argv[1]);
   pot0=atoi(argv[2]);

   if ((method < 1) || (method > 2)) {
      printf("\n");
      printf("Wrong method number.\n");
      printf("\n");
      exit(0);
   } /* if */

   if (pot0 < 8) {
      pot0=8;
   } /* if */


if (method == 1) {   


   printf("\n");
   printf("Apery's constant Zeta(3), method 1\n");
   printf("================================== \n");
   printf("\n");
   printf("%s\n",version_string);
   printf("\n");

   pot=pot0;

   n=1;

   for (i=1; i<=pot; i++) {
      n=2*n;
   } /* for i */

   decimals=n;
   decimals2=decimals;

   frac=0.995;
   decimals=decimals/4;
   decimals=(int)(frac*(double)(decimals-8))-2;
   decimals=4*decimals;

   n1=0;
   n2=(int)((double)(decimals2)/2.90+0.5)+1;

   bigsize=n/4;

   length=n/4;
   ilength=8;
   frac=0.995;

   nn[0]=length;
   nn[1]=ilength;
   nn[2]=length-ilength;
   nn[3]=(int)(frac*(double)(length-ilength))-2;

   lsize=nn[0];


   printf("mulversion = %10d\n",mulversion);
   printf("Power of 2 = %10d\n",pot);
   printf("n          = %10d\n",n);
   printf("FFT        = %10d\n",2*mulversion*bigsize);
   printf("maxsize    = %10d\n",bigsize);
   printf("decimals   = %10d  (%d)\n",decimals,decimals2);
   printf("n1         = %10d\n",n1);
   printf("n2         = %10d\n",n2);
   printf("\n");

   printf("Number of calculated decimals: %10d\n",decimals);
   printf("\n");

   temp=(double)(bigsize);
   mem1=(int)(5.4*1048576.0*log(temp)*temp/(log(1048576.0/8.0)*1048576.0/8.0)+0.5);

   mem2=8*2*2*mulversion*bigsize;
   maxmem=mem1+mem2;
   maxmem=maxmem/1024;

   printf("Max approx memory usage: %10d kbyte;   mulversion: %3d\n",maxmem,mulversion);
   printf("\n");

   time(&start_time);
   printf("Start time: ");
   day_and_time(&start_time);

   time2=clock();

   printf("\n");
   printf("\n");

   biginit2(&rP,bigsize);
   biginit2(&rQ,bigsize);
   biginit2(&rT,bigsize);

   inttobig(&rP,0);
   inttobig(&rQ,0);
   inttobig(&rT,0);

   biginit2(&tmp,bigsize);
   biginit2(&tmp2,bigsize);
   biginit2(&tmp3,bigsize);
   biginit2(&tmp4,bigsize);

   inttobig(&tmp,0);
   inttobig(&tmp2,0);
   inttobig(&tmp3,0);
   inttobig(&tmp4,0);

   printf("Calculating Zeta(3) ...\n");

   fftmaxerror=0.0;

   time1=clock();

   sum_zeta3_1(&rP,&rQ,&rT,n1,n2,bigsize);

   timediff1=clock()-time1;

   printf("\n");

   printf("Time Zeta(3), fast series:      %10.2f\n",(double)(timediff1)/timetic);

   bigfree(&rP);

   bigfree(&tmp3);
   bigfree(&tmp4);

   a = (short *)calloc(lsize+2,sizeof(short));
   b = (short *)calloc(lsize+2,sizeof(short));
   zeta3_result = (short *)calloc(lsize+2,sizeof(short));

   bigass(&tmp,&rT);

   inttobig(&tmp2,64);

   bigmul(&tmp2,&tmp2,&rQ);

   bigfree(&rQ);
   bigfree(&rT);

   lzero(a,nn);
   lzero(b,nn);

   big2l(&tmp,&tmp2,a,b,nn);

   bigfree(&tmp);
   bigfree(&tmp2);

   time1=clock();
       
   ldivv(zeta3_result,a,b,nn);

   timediff1=clock()-time1;
   printf("Time div:                       %10.2f\n",(double)(timediff1)/timetic);

   free(a);
   free(b);

   printf("\n");

   printf("FFT max error (should be < 0.25): %16.14f\n",fftmaxerror);

   timediff2=clock()-time2;
   printf("\n");
   printf("Time TOTAL Zeta(3):             %10.2f\n",(double)(timediff2)/timetic);

   printf("\n");

   time(&stop_time); 
   printf("Stop time:  ");
   day_and_time(&stop_time);

   printf("\n");
   printf("\n");

   elapseddays=get_float_days(&stop_time)-get_float_days(&start_time);

   printf("Elapsed clock time: %18.8lf days = %18.6lf hours = %18.2lf seconds\n",
          elapseddays,24.0*elapseddays,24.0*3600.0*elapseddays);

   printf("\n");

   printf("Calculation finished, output to %s.\n",file_name);

   printf("\n");

   outfile=fopen(file_name,"w");

   lfprint(zeta3_result,nn,outfile);  

   fclose(outfile);


} /* if */


if (method == 2) {   


   printf("\n");
   printf("Apery's constant Zeta(3), method 2\n");
   printf("================================== \n");
   printf("\n");
   printf("%s\n",version_string);
   printf("\n");

   pot=pot0;

   n=1;

   for (i=1; i<=pot; i++) {
      n=2*n;
   } /* for i */

   decimals=n;
   decimals2=decimals;

   frac=0.995;
   decimals=decimals/4;
   decimals=(int)(frac*(double)(decimals-8))-2;
   decimals=4*decimals;

   n1=0;
   n2=(int)((double)(decimals2)/4.90+0.5)+1;

   bigsize=n/4;

   length=n/4;
   ilength=8;
   frac=0.995;

   nn[0]=length;
   nn[1]=ilength;
   nn[2]=length-ilength;
   nn[3]=(int)(frac*(double)(length-ilength))-2;

   lsize=nn[0];

   printf("mulversion = %10d\n",mulversion);
   printf("Power of 2 = %10d\n",pot);
   printf("n          = %10d\n",n);
   printf("FFT        = %10d\n",2*mulversion*bigsize);
   printf("maxsize    = %10d\n",bigsize);
   printf("decimals   = %10d  (%d)\n",decimals,decimals2);
   printf("n1         = %10d\n",n1);
   printf("n2         = %10d\n",n2);
   printf("\n");

   printf("Number of calculated decimals: %10d\n",decimals);
   printf("\n");

   temp=(double)(bigsize);
   mem1=(int)(6.3*1048576.0*log(temp)*temp/(log(1048576.0/8.0)*1048576.0/8.0)+0.5);

   mem2=8*2*2*mulversion*bigsize;
   maxmem=mem1+mem2;
   maxmem=maxmem/1024;

   printf("Max approx memory usage: %10d kbyte;   mulversion: %3d\n",maxmem,mulversion);
   printf("\n");

   time(&start_time);
   printf("Start time: ");
   day_and_time(&start_time);

   time2=clock();

   printf("\n");
   printf("\n");

   biginit2(&rP,bigsize);
   biginit2(&rQ,bigsize);
   biginit2(&rT,bigsize);

   inttobig(&rP,0);
   inttobig(&rQ,0);
   inttobig(&rT,0);

   biginit2(&tmp,bigsize);
   biginit2(&tmp2,bigsize);
   biginit2(&tmp3,bigsize);
   biginit2(&tmp4,bigsize);
   biginit2(&tmp5,bigsize);

   inttobig(&tmp,0);
   inttobig(&tmp2,0);
   inttobig(&tmp3,0);
   inttobig(&tmp4,0);
   inttobig(&tmp5,0);

   printf("Calculating Zeta(3) ...\n");

   fftmaxerror=0.0;

   time1=clock();

   sum_zeta3_2(&rP,&rQ,&rT,n1,n2,bigsize);

   timediff1=clock()-time1;

   printf("\n");

   printf("Time Zeta(3), fast series:      %10.2f\n",(double)(timediff1)/timetic);

   bigfree(&rP);

   bigfree(&tmp3);
   bigfree(&tmp4);
   bigfree(&tmp5);

   a = (short *)calloc(lsize+2,sizeof(short));
   b = (short *)calloc(lsize+2,sizeof(short));
   zeta3_result = (short *)calloc(lsize+2,sizeof(short));

   bigass(&tmp,&rT);

   inttobig(&tmp2,24);

   bigmul(&tmp2,&tmp2,&rQ);

   bigfree(&rQ);
   bigfree(&rT);

   lzero(a,nn);
   lzero(b,nn);

   big2l(&tmp,&tmp2,a,b,nn);

   bigfree(&tmp);
   bigfree(&tmp2);

   time1=clock();
       
   ldivv(zeta3_result,a,b,nn);

   timediff1=clock()-time1;
   printf("Time div:                       %10.2f\n",(double)(timediff1)/timetic);

   free(a);
   free(b);

   printf("\n");

   printf("FFT max error (should be < 0.25): %16.14f\n",fftmaxerror);

   timediff2=clock()-time2;
   printf("\n");
   printf("Time TOTAL Zeta(3):             %10.2f\n",(double)(timediff2)/timetic);

   printf("\n");

   time(&stop_time); 
   printf("Stop time:  ");
   day_and_time(&stop_time);

   printf("\n");
   printf("\n");

   elapseddays=get_float_days(&stop_time)-get_float_days(&start_time);

   printf("Elapsed clock time: %18.8lf days = %18.6lf hours = %18.2lf seconds\n",
          elapseddays,24.0*elapseddays,24.0*3600.0*elapseddays);

   printf("\n");

   printf("Calculation finished, output to %s.\n",file_name);

   printf("\n");

   outfile=fopen(file_name,"w");

   lfprint(zeta3_result,nn,outfile);  

   fclose(outfile);


} /* if */


} /* End */
