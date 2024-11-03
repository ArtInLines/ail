#ifndef __FIXED_POINT_H__
#define __FIXED_POINT_H__

/*

Fixed point arithmetic
======================

Uses the standard method of scaling each number by a static amount - here called FP_SCALE.

For 64-bit backing numbers (fp_t), an FP_SCALE of 10^9 leaves 34 bits for the whole-part.

For 32-bit numbers, an FP_SCALE of e.g. 10^6 leaves only 12 bits left for the whole-part (-2048 -> +2048).

The default chooses FP_SCALE = 10^4 which means 18 bits integer resolution and 4 significant digits.

Depending on your application, this may suffice.

------------------------------------------------

Example definitions:

  For 64-bit numbers
    #define fp_t       int64_t
    #define FP_SCALE   (fp_t)1000000000
    #define FP_FORMAT  "%ld"

  For 32-bit numbers
    #define fp_t       int32_t
    #define FP_SCALE   1000000
    #define FP_FORMAT  "%d"

*/

#include <stdint.h>

#if 0
/* This is the type used for fixed-point arithmetic -- know the type's limits! */
#define fp_t              int64_t
/* This is how much you scale each number. 1 == FP_SCALE */
#define FP_SCALE          (fp_t)1000000000 /* 2^30 - 2^26 - 2^22 - 2^21 - 2^18 - 2^16 - 2^13 - 2^12 - 2^10 - 2^9 */
/* This is the printf-format string for output */
#define FP_FORMAT         "%ld"

#else
  #define fp_t       int32_t
  #define FP_SCALE   10000
  #define FP_FORMAT  "%d"
#endif


/* Convenience macros */
#define int2fp(n)         ((fp_t)(n) * FP_SCALE)
#define fp2int(n)         ((fp_t)(n) / FP_SCALE)
//#define fp(_int, _dec)    (((double)_int==(double)-0) ?  -(_fp(_int, _dec)) : _fp(_int, _dec))
#define fp(_int, _dec)   fp_macro( ((fp_t)_int), ((fp_t)1 ## _dec) )



fp_t  fp_macro(fp_t _int, fp_t _dec); /* constructor with macro-magic */
fp_t  fp_new(fp_t _int, fp_t _dec);   /* constructor                  */

fp_t  fp_inc(fp_t n);                 /* unary, increment             */
fp_t  fp_dec(fp_t n);                 /* unary, decrement             */

fp_t  fp_add(fp_t l, fp_t r);         /* binary, add                  */
fp_t  fp_sub(fp_t l, fp_t r);         /* binary, subtract             */
fp_t  fp_mul(fp_t l, fp_t r);         /* binary, multiply             */
fp_t  fp_div(fp_t l, fp_t r);         /* binary, division             */
fp_t  fp_mod(fp_t l, fp_t r);         /* binary, modulo/remainder     */

char* fp_to_string(fp_t n);           /* to string representation     */
fp_t  fp_from_string(char* s);        /* from string representation   */

#endif /* __FIXED_POINT_H__ */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>     /* atof */


static int _log10(fp_t n);
static fp_t _scale(fp_t n);
static fp_t _get_subtractor(fp_t n);



fp_t fp_macro(fp_t whole, fp_t decimal)
{
  return (whole * FP_SCALE) + _scale(decimal) - _scale(_get_subtractor(decimal));
}


fp_t fp_new(fp_t whole, fp_t decimal)
{
       if (whole < 0)  { return -((whole * FP_SCALE) + _scale(decimal)); }
  else if (whole == 0) { return decimal; }
  else                 { return  ((whole * FP_SCALE) + decimal); }
}


fp_t fp_inc(fp_t n) { return n + FP_SCALE; }
fp_t fp_dec(fp_t n) { return n - FP_SCALE; }


fp_t fp_add(fp_t l, fp_t r) { return l + r; }
fp_t fp_sub(fp_t l, fp_t r) { return l - r; }
fp_t fp_mul(fp_t l, fp_t r) { return (((l * r) + (FP_SCALE/2)) / FP_SCALE); }
fp_t fp_div(fp_t l, fp_t r) { return (((l * FP_SCALE) + (r/2)) / r); }
fp_t fp_mod(fp_t l, fp_t r) { return l - ((l/r) * r); }


char* fp_to_string(fp_t n)
{
  static char buf[64];
  int idx = 0;

  /* add sign if negative */
  if (n < 0)
  {
    buf[idx++] = '-';
    n = -n; /* continue working with absolute value */
  }

  fp_t integer_part  = fp2int(n);
  fp_t fraction_part = n - int2fp(integer_part);
  idx += sprintf(&buf[idx], FP_FORMAT ".", integer_part);
  fp_t tmp = fraction_part;
  while (     (fraction_part != 0)
          && ((fraction_part * 10) < FP_SCALE))
  {
    buf[idx++] = '0';
    fraction_part *= 10;
  }
  sprintf(&buf[idx], FP_FORMAT, tmp);
  return buf;
}

fp_t fp_from_string(char* s)
{
  double f = atof(s);
  int ipart = (int)f;
  int rpart = (int)((FP_SCALE) * (((f<0) ? (-f) : f) - (double)ipart) + 0.5);
  if ((f<0) && (ipart == 0)) rpart = -rpart;
  return fp_new(ipart, rpart);
}


// TODO : make all the numbers below, depend on FP_SCALE

static int _log10(fp_t n)
{
  int ret  = (sizeof(fp_t) == 8) ? 18 : 9;
  fp_t max = (sizeof(fp_t) == 8) ? 1000000000000000000 : 1000000000;
  while (ret)
  {
    if (n >= max) return ret;
    ret -= 1;
    max /= 10;
  }
  return ret;
}


static fp_t _scale(fp_t n)
{
  // log(FP_SCALE, 10) = 9
  static int log10_scale;
  if (log10_scale == 0)
  {
    log10_scale = _log10(FP_SCALE);
  }

  int scale = _log10(n) - log10_scale;

  fp_t ten = (fp_t)10;
  while (scale < 0)
  {
    n *= ten;
    scale += 1;
  }
  while (scale > 0)
  {
    n += 5; /* handle integer rounding (adding 5 before dividing by 10) */
    n /= ten;
    scale -= 1;
  }
  return n;
}


static fp_t _get_subtractor(fp_t n)
{
  fp_t max = (sizeof(fp_t) == 8) ? 100000000000000000 : 1000000000 ;
  //                          fp_t max =  (fp_t)100000000000000000;
  const fp_t ten = 10;
  while (max >= n)
  {
    max /= (fp_t)ten;
  }
  return max * (fp_t)ten;
}




#if 0

static fp_t __get_subtractor(fp_t n)
{
  // TODO optimize for size, this method takes up a lot of space

  // return 10 * ~log10(n)

  //if (n >= (fp_t)10000000000000000000) return (fp_t)100000000000000000000;
  //if (n >= (fp_t)1000000000000000000)  return (fp_t)10000000000000000000;
  if (n >= (fp_t)100000000000000000)   return (fp_t)1000000000000000000;
  if (n >= (fp_t)10000000000000000)    return (fp_t)100000000000000000;
  if (n >= (fp_t)1000000000000000)     return (fp_t)10000000000000000;
  if (n >= (fp_t)100000000000000)      return (fp_t)1000000000000000;
  if (n >= (fp_t)10000000000000)       return (fp_t)100000000000000;
  if (n >= (fp_t)1000000000000)        return (fp_t)10000000000000;
  if (n >= (fp_t)100000000000)         return (fp_t)1000000000000;
  if (n >= (fp_t)10000000000)          return (fp_t)100000000000;
  if (n >= (fp_t)1000000000)           return (fp_t)10000000000;
  if (n >= (fp_t)100000000)            return (fp_t)1000000000;
  if (n >= (fp_t)10000000)             return (fp_t)100000000;
  if (n >= (fp_t)1000000)              return (fp_t)10000000;
  if (n >= (fp_t)100000)               return (fp_t)1000000;
  if (n >= (fp_t)10000)                return (fp_t)100000;
  if (n >= (fp_t)1000)                 return (fp_t)10000;
  if (n >= (fp_t)100)                  return (fp_t)1000;
  if (n >= (fp_t)10)                   return (fp_t)100;
  else                                 return (fp_t)10;
}

static fp_t _scale(fp_t n)
{
  // log(FP_SCALE, 10) = 9

  int scale = _log10(n) - 9;
  switch (scale)
  {
    case -9: return n * 1000000000;
    case -8: return n * 100000000;
    case -7: return n * 10000000;
    case -6: return n * 1000000;
    case -5: return n * 100000;
    case -4: return n * 10000;
    case -3: return n * 1000;
    case -2: return n * 100;
    case -1: return n * 10;
    case 0: return n;
    case 1: return (n + 5) / 10;                 // correct for integer rounding (flooring)
    case 2: return (n + 50) / 100;               // correct for integer rounding (flooring)
    case 3: return (n + 500) / 1000;             // correct for integer rounding (flooring)
    case 4: return (n + 5000) / 10000;           // correct for integer rounding (flooring)
    case 5: return (n + 50000) / 100000;         // correct for integer rounding (flooring)
    case 6: return (n + 500000) / 1000000;       // correct for integer rounding (flooring)
    case 7: return (n + 5000000) / 10000000;     // correct for integer rounding (flooring)
    case 8: return (n + 50000000) / 100000000;   // correct for integer rounding (flooring)
    case 9: return (n + 500000000) / 1000000000; // correct for integer rounding (flooring)
  }
}

int __log10(fp_t n)
{
  if (n >= (fp_t)1000000000000000000)  return 18;
  if (n >= (fp_t)100000000000000000)   return 17;
  if (n >= (fp_t)10000000000000000)    return 16;
  if (n >= (fp_t)1000000000000000)     return 15;
  if (n >= (fp_t)100000000000000)      return 14;
  if (n >= (fp_t)10000000000000)       return 13;
  if (n >= (fp_t)1000000000000)        return 12;
  if (n >= (fp_t)100000000000)         return 11;
  if (n >= (fp_t)10000000000)          return 10;
  if (n >= (fp_t)1000000000)           return 9;
  if (n >= (fp_t)100000000)            return 8;
  if (n >= (fp_t)10000000)             return 7;
  if (n >= (fp_t)1000000)              return 6;
  if (n >= (fp_t)100000)               return 5;
  if (n >= (fp_t)10000)                return 4;
  if (n >= (fp_t)1000)                 return 3;
  if (n >= (fp_t)100)                  return 2;
  if (n >= (fp_t)10)                   return 1;
  else                                 return 0;
}

#endif
