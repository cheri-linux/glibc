/* Copyright (C) 2013-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, see <http://www.gnu.org/licenses/>.  */

#include <math.h>
#include <math_private.h>
#include <libm-alias-float.h>

float
__modff (float x, float *iptr)
{
  /* Google-specific: Fix for clang. */
#if defined __clang__
  if (__isinff (x))
#else
  if (__builtin_isinff (x))
#endif
    {
      *iptr = x;
      return __copysignf (0.0, x);
    }
  /* Google-specific: Fix for clang. */
#if defined __clang__
  else if (__isnanf (x))
#else
  else if (__builtin_isnanf (x))
#endif
    {
      *iptr = NAN;
      return NAN;
    }

  if (x >= 0.0)
    {
      *iptr = __floorf (x);
      return __copysignf (x - *iptr, x);
    }
  else
    {
      *iptr = __ceilf (x);
      return __copysignf (x - *iptr, x);
    }
}
libm_alias_float (__modf, modf)
