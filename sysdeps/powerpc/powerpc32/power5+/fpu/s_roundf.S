/* roundf function.  PowerPC32/power5+ version.
   Copyright (C) 2006-2018 Free Software Foundation, Inc.
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
#include <libm-alias-float.h>

	DOT_MACHINE ("power5")
EALIGN (__roundf, 4, 0)
	frin	fp1, fp1	/* The rounding instructions are double.  */
	frsp	fp1, fp1	/* But we need to set ooverflow for float.  */
	blr
	END (__roundf)

libm_alias_float (__round, round)

