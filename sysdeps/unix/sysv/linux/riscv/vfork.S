/* vfork for Linux, RISC-V version.
   Copyright (C) 2005-2018 Free Software Foundation, Inc.
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

/* vfork() is just a special case of clone().  */

#include <sys/asm.h>
#include <sysdep.h>
#define __ASSEMBLY__
#include <linux/sched.h>
#include <asm/signal.h>

	.text
LEAF (__libc_vfork)
#ifndef __CHERI_PURE_CAPABILITY__
	li	a0, (CLONE_VFORK | CLONE_VM | SIGCHLD)
	mv	a1, sp

	li	a7, __NR_clone
	scall

	bltz	a0, 1f
	ret
#else // __CHERI_PURE_CAPABILITY__
	li	a0, (CLONE_VFORK | CLONE_VM | SIGCHLD)
	cmove	ca1, csp

	li	a7, __NR_clone
	scall

	bltz	a0, 1f
	cret
#endif /* __CHERI_PURE_CAPABILITY__ */
1:	j		__syscall_error

END (__libc_vfork)

weak_alias (__libc_vfork, vfork)
strong_alias (__libc_vfork, __vfork)
libc_hidden_def (__vfork)
