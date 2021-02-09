/* setjmp for RISC-V.
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

#ifndef __CHERI_PURE_CAPABILITY__
ENTRY (_setjmp)
  li	a1, 0
  la	t0, __sigsetjmp
  jr	t0
END (_setjmp)
ENTRY (setjmp)
  li	a1, 1
  /* Fallthrough */
END (setjmp)
ENTRY (__sigsetjmp)
	REG_S ra,  0*SZREG(a0)
	REG_S s0,  1*SZREG(a0)
	REG_S s1,  2*SZREG(a0)
	REG_S s2,  3*SZREG(a0)
	REG_S s3,  4*SZREG(a0)
	REG_S s4,  5*SZREG(a0)
	REG_S s5,  6*SZREG(a0)
	REG_S s6,  7*SZREG(a0)
	REG_S s7,  8*SZREG(a0)
	REG_S s8,  9*SZREG(a0)
	REG_S s9, 10*SZREG(a0)
	REG_S s10,11*SZREG(a0)
	REG_S s11,12*SZREG(a0)
	REG_S sp, 13*SZREG(a0)

#ifndef __riscv_float_abi_soft
	FREG_S fs0, 14*SZREG+ 0*SZFREG(a0)
	FREG_S fs1, 14*SZREG+ 1*SZFREG(a0)
	FREG_S fs2, 14*SZREG+ 2*SZFREG(a0)
	FREG_S fs3, 14*SZREG+ 3*SZFREG(a0)
	FREG_S fs4, 14*SZREG+ 4*SZFREG(a0)
	FREG_S fs5, 14*SZREG+ 5*SZFREG(a0)
	FREG_S fs6, 14*SZREG+ 6*SZFREG(a0)
	FREG_S fs7, 14*SZREG+ 7*SZFREG(a0)
	FREG_S fs8, 14*SZREG+ 8*SZFREG(a0)
	FREG_S fs9, 14*SZREG+ 9*SZFREG(a0)
	FREG_S fs10,14*SZREG+10*SZFREG(a0)
	FREG_S fs11,14*SZREG+11*SZFREG(a0)
#endif

#if !IS_IN (libc) && IS_IN (rtld)
  /* In ld.so we never save the signal mask.  */
  li a0, 0
  ret
#else
  /* Make a tail call to __sigjmp_save; it takes the same args.  */
  j __sigjmp_save
#endif

#else // __CHERI_PURE_CAPABILITY__
ENTRY (_setjmp)
  li	a1, 0
  clgc	ct0, __sigsetjmp
  cjr	ct0
END (_setjmp)
ENTRY (setjmp)
  li	a1, 1
  /* Fallthrough */
END (setjmp)
ENTRY (__sigsetjmp)
	CREG_S cra,  0*SZCREG(ca0)
	CREG_S cs0,  1*SZCREG(ca0)
	CREG_S cs1,  2*SZCREG(ca0)
	CREG_S cs2,  3*SZCREG(ca0)
	CREG_S cs3,  4*SZCREG(ca0)
	CREG_S cs4,  5*SZCREG(ca0)
	CREG_S cs5,  6*SZCREG(ca0)
	CREG_S cs6,  7*SZCREG(ca0)
	CREG_S cs7,  8*SZCREG(ca0)
	CREG_S cs8,  9*SZCREG(ca0)
	CREG_S cs9, 10*SZCREG(ca0)
	CREG_S cs10,11*SZCREG(ca0)
	CREG_S cs11,12*SZCREG(ca0)
	CREG_S csp, 13*SZCREG(ca0)

#ifndef __riscv_float_abi_soft
	FREG_S fs0, 14*SZCREG+ 0*SZFREG(ca0)
	FREG_S fs1, 14*SZCREG+ 1*SZFREG(ca0)
	FREG_S fs2, 14*SZCREG+ 2*SZFREG(ca0)
	FREG_S fs3, 14*SZCREG+ 3*SZFREG(ca0)
	FREG_S fs4, 14*SZCREG+ 4*SZFREG(ca0)
	FREG_S fs5, 14*SZCREG+ 5*SZFREG(ca0)
	FREG_S fs6, 14*SZCREG+ 6*SZFREG(ca0)
	FREG_S fs7, 14*SZCREG+ 7*SZFREG(ca0)
	FREG_S fs8, 14*SZCREG+ 8*SZFREG(ca0)
	FREG_S fs9, 14*SZCREG+ 9*SZFREG(ca0)
	FREG_S fs10,14*SZCREG+10*SZFREG(ca0)
	FREG_S fs11,14*SZCREG+11*SZFREG(ca0)
#endif

#if !IS_IN (libc) && IS_IN (rtld)
  /* In ld.so we never save the signal mask.  */
  li a0, 0
  cret
#else
  /* Make a tail call to __sigjmp_save; it takes the same args.  */
  cllc ct0, __sigjmp_save
  cjr ct0
#endif

#endif // __CHERI_PURE_CAPABILITY__

END (__sigsetjmp)

hidden_def (__sigsetjmp)
weak_alias (_setjmp, __GI__setjmp)
