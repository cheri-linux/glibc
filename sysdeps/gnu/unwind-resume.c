/* Copyright (C) 2003-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Jakub Jelinek <jakub@redhat.com>.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, see <http://www.gnu.org/licenses/>.  */

#include <dlfcn.h>
#include <stdio.h>
#include <unwind.h>
#include <gnu/lib-names.h>
#include <sysdep.h>
#include <unwind-resume.h>


void (*__libgcc_s_resume) (struct _Unwind_Exception *exc)
  attribute_hidden __attribute__ ((noreturn));

static _Unwind_Reason_Code (*libgcc_s_personality) PERSONALITY_PROTO;

void attribute_hidden __attribute__ ((cold))
__libgcc_s_init (void)
{
  void *resume, *personality;
  void *handle;

  /* Use RTLD_NOW here for consistency with pthread_cancel_init.
     RTLD_NOW will rarely make a difference here because unwinding is
     already in progress, so libgcc_s.so has already been loaded if
     its unwinder is used.  */
#ifndef LIBUNWIND_SO
  handle = __libc_dlopen_mode (LIBGCC_S_SO, RTLD_NOW | __RTLD_DLOPEN);

  if (handle == NULL
      || (resume = __libc_dlsym (handle, "_Unwind_Resume")) == NULL
      || (personality = __libc_dlsym (handle, "__gcc_personality_v0")) == NULL)
    __libc_fatal (LIBGCC_S_SO
                  " must be installed for unwinding to work\n");
#else
  handle = __libc_dlopen_mode (LIBUNWIND_SO, RTLD_NOW | __RTLD_DLOPEN);

  if (handle == NULL
      || (resume = __libc_dlsym (handle, "_Unwind_Resume")) == NULL)
    __libc_fatal (LIBUNWIND_SO
                  " must be installed for unwinding to work\n");
#endif
#ifdef PTR_MANGLE
  PTR_MANGLE (resume);
#endif
  __libgcc_s_resume = resume;
#ifndef LIBUNWIND_SO
#ifdef PTR_MANGLE
  PTR_MANGLE (personality);
#endif
  libgcc_s_personality = personality;
#endif
}

#if !HAVE_ARCH_UNWIND_RESUME
void
_Unwind_Resume (struct _Unwind_Exception *exc)
{
  if (__glibc_unlikely (__libgcc_s_resume == NULL))
    __libgcc_s_init ();

  __typeof (__libgcc_s_resume) resume = __libgcc_s_resume;
#ifdef PTR_DEMANGLE
  PTR_DEMANGLE (resume);
#endif
  (*resume) (exc);
}
#endif

_Unwind_Reason_Code
__gcc_personality_v0 PERSONALITY_PROTO
{
  if (__glibc_unlikely (libgcc_s_personality == NULL))
    __libgcc_s_init ();

  __typeof (libgcc_s_personality) personality = libgcc_s_personality;
#ifdef PTR_DEMANGLE
  PTR_DEMANGLE (personality);
#endif
  return (*personality) PERSONALITY_ARGS;
}
