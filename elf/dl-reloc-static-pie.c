/* Support for relocating static PIE.
   Copyright (C) 2017-2018 Free Software Foundation, Inc.
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

#if ENABLE_STATIC_PIE
#include <unistd.h>
#include <ldsodefs.h>
#include "dynamic-link.h"

#ifndef NESTING
# define STATIC_PIE_BOOTSTRAP
# define BOOTSTRAP_MAP (main_map)
# define RESOLVE_MAP(sym, version, flags) BOOTSTRAP_MAP
# include "dynamic-link.h"
#endif /* n NESTING */

/* Relocate static executable with PIE.  */

int
_dl_relocate_static_pie (void)
{
  struct link_map *main_map = _dl_get_dl_main_map ();

#ifdef NESTING
# define STATIC_PIE_BOOTSTRAP
# define BOOTSTRAP_MAP (main_map)
# define RESOLVE_MAP(sym, version, flags) BOOTSTRAP_MAP
# include "dynamic-link.h"
#endif /* NESTING */

  /* Figure out the run-time load address of static PIE.  */
  main_map->l_addr = elf_machine_load_address ();

  /* Read our own dynamic section and fill in the info array.  */
  main_map->l_ld = ((void *) main_map->l_addr + elf_machine_dynamic ());
  elf_get_dynamic_info (main_map, NULL);

# ifdef ELF_MACHINE_BEFORE_RTLD_RELOC
  ELF_MACHINE_BEFORE_RTLD_RELOC (main_map->l_info);
# endif

  /* Relocate ourselves so we can do normal function calls and
     data access using the global offset table.  */
  ELF_DYNAMIC_RELOCATE (main_map, 0, 0, 0
#ifndef NESTING
                        , main_map
#endif
                        );
  main_map->l_relocated = 1;

  return 1;
}
#endif
