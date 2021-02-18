/* syscall error handlers
   Copyright (C) 2011-2018 Free Software Foundation, Inc.
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

#if IS_IN (libc)
# define errno __libc_errno
#endif

ENTRY (__syscall_error)
#ifdef __CHERI_PURE_CAPABILITY__
	cmove ct0, cra
#else
	mv t0, ra
#endif
	/* Fall through to __syscall_set_errno.  */
END (__syscall_error)

/* Non-standard calling convention: argument in a0, return address in t0,
   and clobber only t1.  */
ENTRY (__syscall_set_errno)
	/* We got here because a0 < 0, but only codes in the range [-4095, -1]
	  represent errors.  Otherwise, just return the result normally.  */
	li t1, -4096
	bleu a0, t1, 1f
	neg a0, a0
#ifndef __CHERI_PURE_CAPABILITY__
#if RTLD_PRIVATE_ERRNO
	sw a0, rtld_errno, t1
#elif defined(__PIC__)
	la.tls.ie t1, errno
	add t1, t1, tp
	sw a0, 0(t1)
#else
	lui t1, %tprel_hi(errno)
	add t1, t1, tp, %tprel_add(errno)
	sw a0, %tprel_lo(errno)(t1)
#endif
	li a0, -1
1:	jr t0
#else // __CHERI_PURE_CAPABILITY__
#if RTLD_PRIVATE_ERRNO
	/* TODO Cheri: adapt this */
3:	auipcc ct1, %pcrel_hi(errno)
	csw a0, %pcrel_lo(3b)(ct1)
#elif defined(__PIC__)
	//la.tls.ie t1, errno
	// no idea if that's working
2:	auipcc ct1, %tls_ie_pcrel_hi(errno)
	cld t1, %pcrel_lo(2b)(ct1)
	cincoffset ct1, ctp, t1 //, %tprel_add(errno)
	csw a0, 0(ct1)
#else
	lui t1, %tprel_hi(errno)
	cincoffset ct1, ctp, t1 //, %tprel_add(errno)
	csw a0, %tprel_lo(errno)(ct1)
#endif
	li a0, -1
1:	cjr ct0
#endif
END (__syscall_set_errno)
