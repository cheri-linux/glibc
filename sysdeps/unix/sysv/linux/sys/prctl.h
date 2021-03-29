/* Copyright (C) 1997-2018 Free Software Foundation, Inc.
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

#ifndef _SYS_PRCTL_H
#define _SYS_PRCTL_H	1

#include <features.h>
#include <linux/prctl.h>  /*  The magic values come from here  */
#include <stdint.h>

__BEGIN_DECLS

/* Control process execution.  */
#ifndef __CHERI_PURE_CAPABILITY__
extern int prctl (int __option, ...) __THROW;
#else
extern int prctl (int __option, uintptr_t __arg2, uintptr_t __arg3,
		  uintptr_t __arg4, uintptr_t __arg5) __THROW;

#ifndef __prc
# define __prc(a) ((uintptr_t) (a))
#endif

#define __PRCTL_NARGS_X(a,b,c,d,e,f,...) f
#define __PRCTL_NARGS(...) __PRCTL_NARGS_X (__VA_ARGS__,4,3,2,1,0,)
#define __PRCTL_CONCAT_X(a,b) a##b
#define __PRCTL_CONCAT(a,b) __PRCTL_CONCAT_X (a, b)
#define __PRCTL_DISP(b,...) __PRCTL_CONCAT (b,__PRCTL_NARGS(__VA_ARGS__))(__VA_ARGS__)

#define __prctl_cheri0(option) (prctl) (option, 0, 0, 0, 0)
#define __prctl_cheri1(option,a) (prctl) (option, __prc(a), 0, 0, 0)
#define __prctl_cheri2(option,a,b) (prctl) (option, __prc(a), __prc(b), 0, 0)
#define __prctl_cheri3(option,a,b,c) (prctl) (option, __prc(a), __prc(b), __prc(c), 0)
#define __prctl_cheri4(option,a,b,c,d) (prctl) (option, __prc(a), __prc(b), __prc(c), __prc(d))

#define prctl(...) __PRCTL_DISP (__prctl_cheri, __VA_ARGS__)
#endif

__END_DECLS

#endif  /* sys/prctl.h */
