/* Macros to control TS 18661-3 glibc features on powerpc.
   Copyright (C) 2017-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _BITS_FLOATN_H
#define _BITS_FLOATN_H

#include <features.h>
#include <bits/long-double.h>

#if 0 /* for macro expansion debugging */
#define capture0(sym) typedef int sym##_eq_0
#define capture1(sym) typedef int sym##_eq_1
/*#define capture(sym, val) extern int sym[val]*/
#endif

/* Defined to 1 if the current compiler invocation provides a
   floating-point type with the IEEE 754 binary128 format, and this glibc
   includes corresponding *f128 interfaces for it.  */
#if defined _ARCH_PWR8 && defined __LITTLE_ENDIAN__ && (_CALL_ELF == 2) \
    && defined __FLOAT128__ && !defined __NO_LONG_DOUBLE_MATH && !defined __clang__
# define __HAVE_FLOAT128 1
#else
# define __HAVE_FLOAT128 0
#endif

/* Defined to 1 if __HAVE_FLOAT128 is 1 and the type is ABI-distinct
   from the default float, double and long double types in this glibc.  */
#if __HAVE_FLOAT128
# define __HAVE_DISTINCT_FLOAT128 1
#else
# define __HAVE_DISTINCT_FLOAT128 0
#endif

/* Defined to 1 if the current compiler invocation provides a
   floating-point type with the right format for _Float64x, and this
   glibc includes corresponding *f64x interfaces for it.  */
#define __HAVE_FLOAT64X __HAVE_FLOAT128

/* Defined to 1 if __HAVE_FLOAT64X is 1 and _Float64x has the format
   of long double.  Otherwise, if __HAVE_FLOAT64X is 1, _Float64x has
   the format of _Float128, which must be different from that of long
   double.  */
#define __HAVE_FLOAT64X_LONG_DOUBLE 0

#ifndef __ASSEMBLER__
#if 0 /* for macro expansion debugging */
#if __HAVE_FLOAT128
capture1(i_have_float128);
#else
capture0(i_have_float128);
#endif
#if __HAVE_DISTINCT_FLOAT128
capture1(i_have_distinct_float128);
#else
capture0(i_have_distinct_float128);
#endif
/*capture(i_have_float128, __HAVE_FLOAT128);*/
/*capture(i_have_distinct_float128, __HAVE_DISTINCT_FLOAT128);*/
#endif
#endif

#ifndef __ASSEMBLER__

/* Defined to concatenate the literal suffix to be used with _Float128
   types, if __HAVE_FLOAT128 is 1. */
# if __HAVE_FLOAT128
#  if !__GNUC_PREREQ (7, 0) || defined __cplusplus
/* The literal suffix (f128) exist for powerpc only since GCC 7.0.  */
#   define __f128(x) x##q
#  else
#   define __f128(x) x##f128
#  endif
# endif

/* Defined to a complex binary128 type if __HAVE_FLOAT128 is 1.  */
# if __HAVE_FLOAT128
#  if !defined __clang__ && (!__GNUC_PREREQ (7, 0) || defined __cplusplus)
/* Add a typedef for older GCC compilers which don't natively support
   _Complex _Float128.  */
typedef _Complex float __cfloat128 __attribute__ ((__mode__ (__KC__)));
#   define __CFLOAT128 __cfloat128
#  else
#   define __CFLOAT128 _Complex _Float128
#  endif
# endif

/* The remaining of this file provides support for older compilers.  */
# if __HAVE_FLOAT128

/* The type _Float128 exist for powerpc only since GCC 7.0.  */
#  if !defined __clang__ && (!__GNUC_PREREQ (7, 0) || defined __cplusplus)
typedef __float128 _Float128;
#  endif

/* Builtin __builtin_huge_valf128 doesn't exist before GCC 7.0.  */
#  if !__GNUC_PREREQ (7, 0)
#   define __builtin_huge_valf128() ((_Float128) __builtin_huge_val ())
#  endif

#if defined __clang__

#if 1 /* 6.0 or less */
#define __builtin_huge_valf128() ((_Float128) __builtin_huge_val ())
#define __builtin_inff128() ((_Float128) __builtin_inf ())
#define __builtin_nanf128(x) ((_Float128) __builtin_nan (x))
#define __builtin_nansf128(x) ((_Float128) __builtin_nans (x))

#define __builtin_copysignf128(x,y) ((_Float128) __builtin_copysign ((double)(x),(double)(y)))
#define __builtin_fabsf128(x) ((_Float128) __builtin_fabs ((double)(x)))
#define __builtin_signbitf128(x) (__builtin_signbit (x))
#endif

#else /* GCC */

/* The following builtins (suffixed with 'q') are available in GCC >= 6.2,
   which is the minimum version required for float128 support on powerpc64le.
   Since GCC 7.0 the builtins suffixed with f128 are also available, then
   there is no need to redefined them.  */
#  if !__GNUC_PREREQ (7, 0)
#   define __builtin_copysignf128 __builtin_copysignq
#   define __builtin_fabsf128 __builtin_fabsq
#   define __builtin_inff128 __builtin_infq
#   define __builtin_nanf128 __builtin_nanq
#   define __builtin_nansf128 __builtin_nansq
#  endif

#endif /* GCC or clang */

/* In math/math.h, __MATH_TG will expand signbit to __builtin_signbit*,
   e.g.: __builtin_signbitf128, before GCC 6.  However, there has never
   been a __builtin_signbitf128 in GCC and the type-generic builtin is
   only available since GCC 6.  */
#  if !__GNUC_PREREQ (6, 0)
#   define __builtin_signbitf128 __signbitf128
#  endif

# endif

#if !__HAVE_FLOAT128 && defined __clang__
typedef long double _Float128;
#endif /* clang fallback */

#endif /* !__ASSEMBLER__.  */

#include <bits/floatn-common.h>

#endif /* _BITS_FLOATN_H */
