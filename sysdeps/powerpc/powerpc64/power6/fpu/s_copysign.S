/* copysign().  PowerPC64/POWER6 version.
   Copyright (C) 2010-2018 Free Software Foundation, Inc.
   Contributed by Luis Machado <luisgpm@br.ibm.com>.
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

#include <sysdep.h>
#include <math_ldbl_opt.h>
#include <libm-alias-float.h>
#include <libm-alias-double.h>

/* double [f1] copysign (double [f1] x, double [f2] y);
   copysign(x,y) returns a value with the magnitude of x and
   with the sign bit of y.  */

	.section    ".text"
	.type	    __copysign, @function
	DOT_MACHINE ("power6")
ENTRY_TOCLESS (__copysign, 4)
	CALL_MCOUNT 0
	fcpsgn	fp1,fp2,fp1
	blr
END (__copysign)

hidden_def (__copysign)
libm_alias_double (__copysign, copysign)

/* It turns out that the 'double' version will also always work for
   single-precision.  */
strong_alias (__copysign, __copysignf)
hidden_def (__copysignf)
libm_alias_float (__copysign, copysign)

#if LONG_DOUBLE_COMPAT (libc, GLIBC_2_0)
compat_symbol (libc, copysign, copysignl, GLIBC_2_0);
#endif
