/* Set current context.
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

/*  int __setcontext (const ucontext_t *ucp)

  Restores the machine context in UCP and thereby resumes execution
  in that context.

  This implementation is intended to be used for *synchronous* context
  switches only.  Therefore, it does not have to restore anything
  other than the PRESERVED state.  */

	.text
LEAF (__setcontext)

#ifndef __CHERI_PURE_CAPABILITY__
	mv	t0, a0	/* Save ucp into t0.  */

/* rt_sigprocmask (SIG_SETMASK, &ucp->uc_sigmask, NULL, _NSIG8) */
	li	a3, _NSIG8
	mv	a2, zero
	add     a1, a0, UCONTEXT_SIGMASK
	li	a0, SIG_SETMASK

	li	a7, SYS_ify (rt_sigprocmask)
	scall

	bltz	a0, 99f

	cfi_def_cfa (t0, 0)

#ifndef __riscv_float_abi_soft
	lw	t1, MCONTEXT_FSR(t0)

	RESTORE_FP_REG_CFI (fs0,   8, t0)
	RESTORE_FP_REG_CFI (fs1,   9, t0)
	RESTORE_FP_REG_CFI (fs2,  18, t0)
	RESTORE_FP_REG_CFI (fs3,  19, t0)
	RESTORE_FP_REG_CFI (fs4,  20, t0)
	RESTORE_FP_REG_CFI (fs5,  21, t0)
	RESTORE_FP_REG_CFI (fs6,  22, t0)
	RESTORE_FP_REG_CFI (fs7,  23, t0)
	RESTORE_FP_REG_CFI (fs8,  24, t0)
	RESTORE_FP_REG_CFI (fs9,  25, t0)
	RESTORE_FP_REG_CFI (fs10, 26, t0)
	RESTORE_FP_REG_CFI (fs11, 27, t0)

	fssr	t1
#endif /* __riscv_float_abi_soft */

	/* Note the contents of argument registers will be random
	   unless makecontext() has been called.  */
	RESTORE_INT_REG     (t1,   0, t0)
	RESTORE_INT_REG_CFI (ra,   1, t0)
	RESTORE_INT_REG     (sp,   2, t0)
	RESTORE_INT_REG_CFI (s0,   8, t0)
	RESTORE_INT_REG_CFI (s1,   9, t0)
	RESTORE_INT_REG     (a0,  10, t0)
	RESTORE_INT_REG     (a1,  11, t0)
	RESTORE_INT_REG     (a2,  12, t0)
	RESTORE_INT_REG     (a3,  13, t0)
	RESTORE_INT_REG     (a4,  14, t0)
	RESTORE_INT_REG     (a5,  15, t0)
	RESTORE_INT_REG     (a6,  16, t0)
	RESTORE_INT_REG     (a7,  17, t0)
	RESTORE_INT_REG_CFI (s2,  18, t0)
	RESTORE_INT_REG_CFI (s3,  19, t0)
	RESTORE_INT_REG_CFI (s4,  20, t0)
	RESTORE_INT_REG_CFI (s5,  21, t0)
	RESTORE_INT_REG_CFI (s6,  22, t0)
	RESTORE_INT_REG_CFI (s7,  23, t0)
	RESTORE_INT_REG_CFI (s8,  24, t0)
	RESTORE_INT_REG_CFI (s9,  25, t0)
	RESTORE_INT_REG_CFI (s10, 26, t0)
	RESTORE_INT_REG_CFI (s11, 27, t0)

	jr	t1

99:	j	__syscall_error
#else /* __CHERI_PURE_CAPABILITY__ */
	cmove	ct0, ca0	/* Save ucp into ct0.  */

/* rt_sigprocmask (SIG_SETMASK, &ucp->uc_sigmask, NULL, _NSIG8) */
	li	a3, _NSIG8
	cmove	ca2, cnull
	cincoffset ca1, ca0, UCONTEXT_SIGMASK
	li	a0, SIG_SETMASK

	li	a7, SYS_ify (rt_sigprocmask)
	scall

	bltz	a0, 99f

	cfi_def_cfa (ct0, 0)

#ifndef __riscv_float_abi_soft
	clw	t1, MCONTEXT_FSR(ct0)

	RESTORE_FP_REG_CFI (fs0,   8, ct0)
	RESTORE_FP_REG_CFI (fs1,   9, ct0)
	RESTORE_FP_REG_CFI (fs2,  18, ct0)
	RESTORE_FP_REG_CFI (fs3,  19, ct0)
	RESTORE_FP_REG_CFI (fs4,  20, ct0)
	RESTORE_FP_REG_CFI (fs5,  21, ct0)
	RESTORE_FP_REG_CFI (fs6,  22, ct0)
	RESTORE_FP_REG_CFI (fs7,  23, ct0)
	RESTORE_FP_REG_CFI (fs8,  24, ct0)
	RESTORE_FP_REG_CFI (fs9,  25, ct0)
	RESTORE_FP_REG_CFI (fs10, 26, ct0)
	RESTORE_FP_REG_CFI (fs11, 27, ct0)

	fssr	t1
#endif /* __riscv_float_abi_soft */

	/* Note the contents of argument registers will be random
	   unless makecontext() has been called.  */
	RESTORE_CAP_REG     (ct1,   0, ct0)
	RESTORE_CAP_REG_CFI (cra,   1, ct0)
	RESTORE_CAP_REG     (csp,   2, ct0)
	RESTORE_CAP_REG_CFI (cs0,   8, ct0)
	RESTORE_CAP_REG_CFI (cs1,   9, ct0)
	RESTORE_CAP_REG     (ca0,  10, ct0)
	RESTORE_CAP_REG     (ca1,  11, ct0)
	RESTORE_CAP_REG     (ca2,  12, ct0)
	RESTORE_CAP_REG     (ca3,  13, ct0)
	RESTORE_CAP_REG     (ca4,  14, ct0)
	RESTORE_CAP_REG     (ca5,  15, ct0)
	RESTORE_CAP_REG     (ca6,  16, ct0)
	RESTORE_CAP_REG     (ca7,  17, ct0)
	RESTORE_CAP_REG_CFI (cs2,  18, ct0)
	RESTORE_CAP_REG_CFI (cs3,  19, ct0)
	RESTORE_CAP_REG_CFI (cs4,  20, ct0)
	RESTORE_CAP_REG_CFI (cs5,  21, ct0)
	RESTORE_CAP_REG_CFI (cs6,  22, ct0)
	RESTORE_CAP_REG_CFI (cs7,  23, ct0)
	RESTORE_CAP_REG_CFI (cs8,  24, ct0)
	RESTORE_CAP_REG_CFI (cs9,  25, ct0)
	RESTORE_CAP_REG_CFI (cs10, 26, ct0)
	RESTORE_CAP_REG_CFI (cs11, 27, ct0)

	cjr	ct1

99:	cllc	ct1, __syscall_error
	cjr		ct1

#endif /* __CHERI_PURE_CAPABILITY__ */

PSEUDO_END (__setcontext)
weak_alias (__setcontext, setcontext)

LEAF (__start_context)

#ifndef __CHERI_PURE_CAPABILITY__
	/* Terminate call stack by noting ra == 0.  Happily, s0 == 0 here.  */
	cfi_register (ra, s0)

	/* Call the function passed to makecontext.  */
	jalr	s1

	/* Invoke subsequent context if present, else exit(0).  */
	mv	a0, s2
	beqz	s2, 1f
	jal	__setcontext
1:	la	t0, exit
	jr	t0

#else /* __CHERI_PURE_CAPABILITY__ */

	/* Terminate call stack by noting cra == 0.  Happily, cs0 == 0 here.  */
	cfi_register (cra, cs0)

	/* Call the function passed to makecontext.  */
	cjalr	cs1

	/* Invoke subsequent context if present, else exit(0).  */
	cmove	ca0, cs2
	/* TODO: Do we have to check if full cap reg cs2 equals cnull or is the following sufficient? */
	beqz	s2, 1f
	cllc 	ct0, __setcontext
	cjalr	ct0
1:	clgc ct0, exit
	cjr	ct0

#endif /* __CHERI_PURE_CAPABILITY__ */

PSEUDO_END (__start_context)
