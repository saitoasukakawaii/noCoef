/***************************************************************************/
/*                                                                         */
/*  Program: tools.h                                                       */
/*  Version: 2.0                                                           */
/*  By: Mette Olufsen, Math-Tech                                           */
/*  Date: 14. Jan. 1997                                                    */
/*                                                                         */
/*  A header file for the auxilliary functions in the module tools.C.      */
/*  Several of these are taken from Numerical Recipes in C, namely the     */
/*  routines used for finding zeros of a function. Either in several       */
/*  dimensions, namely ludcmp, lubksb, zeroludcmp, or in one dimension,    */
/*  zero_1d.                                                               */
/*                                                                         */
/***************************************************************************/

// $Id: tools.h,v 1.5 2010-10-20 15:06:06 mette Exp $

#ifndef _TOOLS_H
#define _TOOLS_H

#include <set>
#include <string>
using namespace std;

void   error        (const char *s, const char *u);
void   ludcmp       (double *a[], int n, int indx[], double *d);
void   lubksb       (double *a[], int n, int indx[], double b[]);
int    zero         (double  *x, int n, double tolx, double tolf,
                     double fvec[], double *fjac[]);
bool   zero_1d      (double *x, double f, double df, double tolx);

void get_ID(const string &FileName, set<int>& ID_Out, set<int>& ID_Bif, set<int> &ID_Merge);
void thomas (double *a, double *b, double *c, double *x, int n);
inline double sq(double x) { return x*x; }
inline double cu(double x) { return x*x*x; }


#endif
