/* system call interface.  Linux/RISC-V version.
   Copyright (C) 2001-2018 Free Software Foundation, Inc.
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

/* TODO Cheri: Adapt this file */

long int
syscall (long int syscall_number, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3,
	 uintptr_t arg4, uintptr_t arg5, uintptr_t arg6, uintptr_t arg7)
{
  long int ret;
  INTERNAL_SYSCALL_DECL (err);

  ret = INTERNAL_SYSCALL_NCS (syscall_number, err, 7, arg1, arg2, arg3, arg4,
			      arg5, arg6, arg7);

  if (INTERNAL_SYSCALL_ERROR_P (ret, err))
    return __syscall_error (ret);

  return ret;
}
