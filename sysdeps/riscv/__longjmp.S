/* longjmp, RISC-V version.
   Copyright (C) 1996-2018 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <sysdep.h>
#include <sys/asm.h>

ENTRY (__longjmp)
#ifndef __CHERI_PURE_CAPABILITY__
	REG_L ra,  0*SZREG(a0)
	REG_L s0,  1*SZREG(a0)
	REG_L s1,  2*SZREG(a0)
	REG_L s2,  3*SZREG(a0)
	REG_L s3,  4*SZREG(a0)
	REG_L s4,  5*SZREG(a0)
	REG_L s5,  6*SZREG(a0)
	REG_L s6,  7*SZREG(a0)
	REG_L s7,  8*SZREG(a0)
	REG_L s8,  9*SZREG(a0)
	REG_L s9, 10*SZREG(a0)
	REG_L s10,11*SZREG(a0)
	REG_L s11,12*SZREG(a0)
	REG_L sp, 13*SZREG(a0)

#ifndef __riscv_float_abi_soft
	FREG_L fs0, 14*SZREG+ 0*SZFREG(a0)
	FREG_L fs1, 14*SZREG+ 1*SZFREG(a0)
	FREG_L fs2, 14*SZREG+ 2*SZFREG(a0)
	FREG_L fs3, 14*SZREG+ 3*SZFREG(a0)
	FREG_L fs4, 14*SZREG+ 4*SZFREG(a0)
	FREG_L fs5, 14*SZREG+ 5*SZFREG(a0)
	FREG_L fs6, 14*SZREG+ 6*SZFREG(a0)
	FREG_L fs7, 14*SZREG+ 7*SZFREG(a0)
	FREG_L fs8, 14*SZREG+ 8*SZFREG(a0)
	FREG_L fs9, 14*SZREG+ 9*SZFREG(a0)
	FREG_L fs10,14*SZREG+10*SZFREG(a0)
	FREG_L fs11,14*SZREG+11*SZFREG(a0)
#endif

	seqz a0, a1
	add  a0, a0, a1   # a0 = (a1 == 0) ? 1 : a1
	ret
#else //__CHERI_PURE_CAPABILITY__
	CREG_L cra,  0*SZCREG(ca0)
	CREG_L cs0,  1*SZCREG(ca0)
	CREG_L cs1,  2*SZCREG(ca0)
	CREG_L cs2,  3*SZCREG(ca0)
	CREG_L cs3,  4*SZCREG(ca0)
	CREG_L cs4,  5*SZCREG(ca0)
	CREG_L cs5,  6*SZCREG(ca0)
	CREG_L cs6,  7*SZCREG(ca0)
	CREG_L cs7,  8*SZCREG(ca0)
	CREG_L cs8,  9*SZCREG(ca0)
	CREG_L cs9, 10*SZCREG(ca0)
	CREG_L cs10,11*SZCREG(ca0)
	CREG_L cs11,12*SZCREG(ca0)
	CREG_L csp, 13*SZCREG(ca0)

#ifndef __riscv_float_abi_soft
	FREG_L fs0, 14*SZCREG+ 0*SZFREG(ca0)
	FREG_L fs1, 14*SZCREG+ 1*SZFREG(ca0)
	FREG_L fs2, 14*SZCREG+ 2*SZFREG(ca0)
	FREG_L fs3, 14*SZCREG+ 3*SZFREG(ca0)
	FREG_L fs4, 14*SZCREG+ 4*SZFREG(ca0)
	FREG_L fs5, 14*SZCREG+ 5*SZFREG(ca0)
	FREG_L fs6, 14*SZCREG+ 6*SZFREG(ca0)
	FREG_L fs7, 14*SZCREG+ 7*SZFREG(ca0)
	FREG_L fs8, 14*SZCREG+ 8*SZFREG(ca0)
	FREG_L fs9, 14*SZCREG+ 9*SZFREG(ca0)
	FREG_L fs10,14*SZCREG+10*SZFREG(ca0)
	FREG_L fs11,14*SZCREG+11*SZFREG(ca0)
#endif

	seqz a0, a1
	add  a0, a0, a1   # a0 = (a1 == 0) ? 1 : a1
	cret
#endif // __CHERI_PURE_CAPABILITY__

END (__longjmp)
