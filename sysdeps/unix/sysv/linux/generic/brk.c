/* Copyright (C) 2011-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Chris Metcalf <cmetcalf@tilera.com>, 2011.

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

#include <errno.h>
#include <unistd.h>
#include <sysdep.h>

#ifdef __CHERI_PURE_CAPABILITY__
#include <cheric.h>
//void *__brkstart = NULL;
#define BRKSTART 0x0
#endif

/* This must be initialized data because commons can't have aliases.  */
void *__curbrk = NULL;

/* Old braindamage in GCC's crtstuff.c requires this symbol in an attempt
   to work around different old braindamage in the old Linux ELF dynamic
   linker.  */
weak_alias (__curbrk, ___brk_addr)

int
__brk (void *addr)
{
  INTERNAL_SYSCALL_DECL (err);

  void *tmpbrk = NULL;

  tmpbrk = (void *) INTERNAL_SYSCALL (brk, err, 1, addr);
  if (tmpbrk < addr)
    {
      __set_errno (ENOMEM);
      return -1;
    }

  #ifdef __CHERI_PURE_CAPABILITY__
  __curbrk = cheri_setaddress(CHERI_CAST (BRKSTART, tmpbrk - BRKSTART), tmpbrk);
  #else
  __curbrk = tmpbrk;
  #endif

  return 0;
}
weak_alias (__brk, brk)
