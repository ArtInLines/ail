/*
*** Common Math functions ***
* The following macros are provided:
* @Note: None of these macros guarantuee safety in regards to side-effects, so be aware to avoid something like ail_min(x++, --y);
  * ail_max(a, b):              Get the maximum of two values
  * ail_min(a, b):              Get the minimum of two values
  * ail_clamp(x, min, max):     Returns the closest value to x in the range [min; max]
  * ail_is_2power(x):           Returns whether x is a power of 2
  * ail_next_2power(x):         Get the next highest value above x that is a power of 2 (if x isn't already a power of 2)
  * ail_lerp(t, min, max):      Linearly interpolate between min and max
  * ail_inv_lerp(x, min, max):  Does the inverse of a linear interpolation, returning the interpolater, such that the following holds: ail_lerp(ail_inv_lerp(x, min, max), min, max) == x
*/

#ifndef _AIL_BASE_MATH_H_
#define _AIL_BASE_MATH_H_

// @Note: Not safe to use with expressions, that have side-effects (e.g. ail_max(x++, y++))
#define ail_max(a, b) (((a) > (b)) ? (a) : (b))
#define ail_min(a, b) (((a) < (b)) ? (a) : (b))
#define ail_clamp(x, min, max) ((x) > (max) ? (max) : ((x) < (min) ? (min) : (x)))

#define ail_is_2power_pos(x) (x && ((x & (x - 1)) == 0))
#define ail_is_2power(x)     ((x < 0) ? ail_is_2power_pos(-(x)) : ail_is_2power_pos(x))
#define ail_next_2power_pos(x, out) do {                                                                                                      \
        out = (x) - 1;                                                                                                                        \
        for (size_t _ail_next_2power_shift_ = 1; _ail_next_2power_shift_ < 8 * sizeof(x); _ail_next_2power_shift_ += _ail_next_2power_shift_) \
            out |= out >> _ail_next_2power_shift_;                                                                                            \
        out += (out+1 <= 1) + 1;                                                                                                              \
    } while(0)
#define ail_next_2power(x, out) do { if (x >= 0) ail_next_2power_pos(x, out); else { ail_next_2power_pos(-(x), out); out = -out; } } while(0)

// ail_lerp(ail_inv_lerp(x, min, max), min, max) = x
#define ail_lerp(t, min, max) ((min) + (t)*((max) - (min)))
#define ail_inv_lerp(x, min, max) (((double)(x) - (double)(min)) / ((double)(max) - (double)(min)))

#endif // _AIL_BASE_MATH_H_
