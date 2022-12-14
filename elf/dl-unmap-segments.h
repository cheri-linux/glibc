/* Unmap a shared object's segments.  Generic version.
   Copyright (C) 2014-2018 Free Software Foundation, Inc.
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

#ifndef _DL_UNMAP_SEGMENTS_H
#define _DL_UNMAP_SEGMENTS_H	1

#include <link.h>
#include <sys/mman.h>

#include <cheric.h>
// TODO Cheri CHERI_CAST bounds?

/* _dl_map_segments ensures that any whole pages in gaps between segments
   are filled in with PROT_NONE mappings.  So we can just unmap the whole
   range in one fell swoop.  */

static __always_inline void
_dl_unmap_segments (struct link_map *l)
{
  __munmap ((void *) CHERI_CAST(l->l_map_start, -1), l->l_map_end - l->l_map_start);
}

#endif  /* dl-unmap-segments.h */
