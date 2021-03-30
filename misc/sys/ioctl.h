/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
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

#ifndef	_SYS_IOCTL_H
#define	_SYS_IOCTL_H	1

#include <features.h>

__BEGIN_DECLS

#include <stdint.h>

/* Get the list of `ioctl' requests and related constants.  */
#include <bits/ioctls.h>

/* Define some types used by `ioctl' requests.  */
#include <bits/ioctl-types.h>

/* On a Unix system, the system <sys/ioctl.h> probably defines some of
   the symbols we define in <sys/ttydefaults.h> (usually with the same
   values).  The code to generate <bits/ioctls.h> has omitted these
   symbols to avoid the conflict, but a Unix program expects <sys/ioctl.h>
   to define them, so we must include <sys/ttydefaults.h> here.  */
#include <sys/ttydefaults.h>

/* Perform the I/O control operation specified by REQUEST on FD.
   One argument may follow; its presence and type depend on REQUEST.
   Return value depends on REQUEST.  Usually -1 indicates error.  */
#ifndef __CHERI_PURE_CAPABILITY__
extern int ioctl (int __fd, unsigned long int __request, ...) __THROW;
#else
extern int ioctl (int __fd, unsigned long int __request,
		  void *__argp) __THROW;

#define __IOCTL_NARGS_X(a,b,c,d,...) d
#define __IOCTL_NARGS(...) __IOCTL_NARGS_X (__VA_ARGS__,1,0,)
#define __IOCTL_CONCAT_X(a,b) a##b
#define __IOCTL_CONCAT(a,b) __IOCTL_CONCAT_X (a, b)
#define __IOCTL_DISP(b,...) __IOCTL_CONCAT (b,__IOCTL_NARGS(__VA_ARGS__))(__VA_ARGS__)

#define __ioctl_cheri0(fd,request) (ioctl) (fd, request, 0)
#define __ioctl_cheri1(fd,request,argp) (ioctl) (fd, request, argp)

#define ioctl(...) __IOCTL_DISP (__ioctl_cheri, __VA_ARGS__)
#endif

__END_DECLS

#endif /* sys/ioctl.h */
