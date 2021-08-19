/* Copy memory to memory until the specified number of bytes
   has been copied.  Overlap is NOT handled correctly.
   Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Torbjorn Granlund (tege@sics.se).

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

#include <string.h>
#include <memcopy.h>
#include <stdint.h>

#undef memcpy

#ifndef __CHERI_PURE_CAPABILITY__
void *
memcpy (void *dstpp, const void *srcpp, size_t len)
{
  int long dstp = (int long) dstpp;
  int long srcp = (int long) srcpp;

  /* Copy from the beginning to the end.  */

  /* If there not too few bytes to copy, use word copy.  */
  if (len >= OP_T_THRES)
    {
      /* Copy just a few bytes to make DSTP aligned.  */
      len -= (-dstp) % OPSIZ;
      BYTE_COPY_FWD (dstp, srcp, (-dstp) % OPSIZ);

      /* Copy whole pages from SRCP to DSTP by virtual address manipulation,
	 as much as possible.  */

      PAGE_COPY_FWD_MAYBE (dstp, srcp, len, len);

      /* Copy from SRCP to DSTP taking advantage of the known alignment of
	 DSTP.  Number of bytes remaining is put in the third argument,
	 i.e. in LEN.  This number may vary from machine to machine.  */

      WORD_COPY_FWD (dstp, srcp, len, len);

      /* Fall out and copy the tail.  */
    }

  /* There are just a few bytes to copy.  Use byte memory operations.  */
  BYTE_COPY_FWD (dstp, srcp, len);

  return dstpp;
}

#else 
#include <cheric.h>
#include <cherireg.h>
#define __CAP_SIZE (_RISCV_SZCAP/8)

void *
memcpy (void *dstpp, const void *srcpp, size_t len)
{
	unsigned char *d = dstpp;
	const unsigned char *s = srcpp;
	for (; (unsigned long)s % __CAP_SIZE && len; len--) *d++ = *s++;
	if ((unsigned long)d % __CAP_SIZE == 0) {
		// source and destination are aligned
		for (; len >= __CAP_SIZE;  s += __CAP_SIZE, d += __CAP_SIZE, len -= __CAP_SIZE) {
			*(void**)d = *(void**)s;
		}
	}
	for (; len; len--) *d++ = *s++;

  return dstpp;
}

void *memcpy_reverse(void *dstpp, const void *srcpp, size_t len)
{
	unsigned char *d = dstpp;
	const unsigned char *s = srcpp;

	for (d += len, s += len; (unsigned long)s % __CAP_SIZE && len; len--) *--d = *--s;
	if ((unsigned long)d % __CAP_SIZE == 0) {
		// source and destination are aligned
		for (; len >= __CAP_SIZE;  len -= __CAP_SIZE) {
			s -= __CAP_SIZE;
			d -= __CAP_SIZE;
			*(void**)d = *(void**)s;
		}
	}
	for (; len; len--) *--d = *--s;
	return dstpp;
}


#endif /* __CHERI_PURE_CAPABILITY__ */
libc_hidden_builtin_def (memcpy)
