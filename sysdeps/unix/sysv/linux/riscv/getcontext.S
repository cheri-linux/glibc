/* Save current context.
   Copyright (C) 2009-2018 Free Software Foundation, Inc.
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

#include "ucontext-macros.h"

/* int getcontext (ucontext_t *ucp) */

	.text
LEAF (__getcontext)
#ifndef __CHERI_PURE_CAPABILITY__
	SAVE_INT_REG (ra,   0, a0)
	SAVE_INT_REG (ra,   1, a0)
	SAVE_INT_REG (sp,   2, a0)
	SAVE_INT_REG (s0,   8, a0)
	SAVE_INT_REG (s1,   9, a0)
	SAVE_INT_REG (x0,  10, a0)	/* return 0 by overwriting a0.  */
	SAVE_INT_REG (s2,  18, a0)
	SAVE_INT_REG (s3,  19, a0)
	SAVE_INT_REG (s4,  20, a0)
	SAVE_INT_REG (s5,  21, a0)
	SAVE_INT_REG (s6,  22, a0)
	SAVE_INT_REG (s7,  23, a0)
	SAVE_INT_REG (s8,  24, a0)
	SAVE_INT_REG (s9,  25, a0)
	SAVE_INT_REG (s10, 26, a0)
	SAVE_INT_REG (s11, 27, a0)
#else // __CHERI_PURE_CAPABILITY__
	SAVE_CAP_REG (cra,   0, ca0)
	SAVE_CAP_REG (cra,   1, ca0)
	SAVE_CAP_REG (csp,   2, ca0)
	SAVE_CAP_REG (cs0,   8, ca0)
	SAVE_CAP_REG (cs1,   9, ca0)
	SAVE_CAP_REG (c0,  10, ca0)	/* return 0 by overwriting a0.  */
	SAVE_CAP_REG (cs2,  18, ca0)
	SAVE_CAP_REG (cs3,  19, ca0)
	SAVE_CAP_REG (cs4,  20, ca0)
	SAVE_CAP_REG (cs5,  21, ca0)
	SAVE_CAP_REG (cs6,  22, ca0)
	SAVE_CAP_REG (cs7,  23, ca0)
	SAVE_CAP_REG (cs8,  24, ca0)
	SAVE_CAP_REG (cs9,  25, ca0)
	SAVE_CAP_REG (cs10, 26, ca0)
	SAVE_CAP_REG (cs11, 27, ca0)
#endif

#ifndef __riscv_float_abi_soft
	frsr	a1 /* read fcsr */

#ifndef __CHERI_PURE_CAPABILITY__
	SAVE_FP_REG (fs0,   8, a0)
	SAVE_FP_REG (fs1,   9, a0)
	SAVE_FP_REG (fs2,  18, a0)
	SAVE_FP_REG (fs3,  19, a0)
	SAVE_FP_REG (fs4,  20, a0)
	SAVE_FP_REG (fs5,  21, a0)
	SAVE_FP_REG (fs6,  22, a0)
	SAVE_FP_REG (fs7,  23, a0)
	SAVE_FP_REG (fs8,  24, a0)
	SAVE_FP_REG (fs9,  25, a0)
	SAVE_FP_REG (fs10, 26, a0)
	SAVE_FP_REG (fs11, 27, a0)
	
	sw	a1, MCONTEXT_FSR(a0)

#else
	SAVE_FP_REG (fs0,   8, ca0)
	SAVE_FP_REG (fs1,   9, ca0)
	SAVE_FP_REG (fs2,  18, ca0)
	SAVE_FP_REG (fs3,  19, ca0)
	SAVE_FP_REG (fs4,  20, ca0)
	SAVE_FP_REG (fs5,  21, ca0)
	SAVE_FP_REG (fs6,  22, ca0)
	SAVE_FP_REG (fs7,  23, ca0)
	SAVE_FP_REG (fs8,  24, ca0)
	SAVE_FP_REG (fs9,  25, ca0)
	SAVE_FP_REG (fs10, 26, ca0)
	SAVE_FP_REG (fs11, 27, ca0)

	csw	a1, MCONTEXT_FSR(ca0)
#endif /* __CHERI_PURE_CAPABILITY__ */
#endif /* __riscv_float_abi_soft */

/* rt_sigprocmask (SIG_BLOCK, NULL, &ucp->uc_sigmask, _NSIG8) */
#ifndef __CHERI_PURE_CAPABILITY__
	li	a3, _NSIG8
	add     a2, a0, UCONTEXT_SIGMASK
	mv	a1, zero
	li	a0, SIG_BLOCK

	li	a7, SYS_ify (rt_sigprocmask)
	scall
	bltz	a0, 99f

	ret

99:	j	__syscall_error
#else /* __CHERI_PURE_CAPABILITY__ */
	li	a3, _NSIG8
	cincoffset ca2, ca0, UCONTEXT_SIGMASK
	cmove	ca1, cnull
	li	a0, SIG_BLOCK

	li	a7, SYS_ify (rt_sigprocmask)
	scall
	bltz	a0, 99f

	cret

99:	cllc	ct0, __syscall_error
	cjr		ct0
#endif /* __CHERI_PURE_CAPABILITY__ */

PSEUDO_END (__getcontext)

weak_alias (__getcontext, getcontext)
