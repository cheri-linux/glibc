/* Machine-dependent ELF dynamic relocation inline functions.  RISC-V version.
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

#ifndef dl_machine_h
#define dl_machine_h

#define ELF_MACHINE_NAME "RISC-V"

#include <entry.h>
#include <elf/elf.h>
#include <sys/asm.h>
#include <dl-tls.h>

#include <cheric.h>
// TODO Cheri CHERI_CAST bounds?
// TODO Cheri CHERI_FN_CAST bounds?

#ifndef _RTLD_PROLOGUE
# define _RTLD_PROLOGUE(entry)						\
	".globl\t" __STRING (entry) "\n\t"				\
	".type\t" __STRING (entry) ", @function\n"			\
	__STRING (entry) ":\n\t"
#endif

#ifndef _RTLD_EPILOGUE
# define _RTLD_EPILOGUE(entry)						\
	".size\t" __STRING (entry) ", . - " __STRING (entry) "\n\t"
#endif

#define ELF_MACHINE_JMP_SLOT R_RISCV_JUMP_SLOT

#define elf_machine_type_class(type)				\
  ((ELF_RTYPE_CLASS_PLT * ((type) == ELF_MACHINE_JMP_SLOT	\
     || (__WORDSIZE == 32 && (type) == R_RISCV_TLS_DTPREL32)	\
     || (__WORDSIZE == 32 && (type) == R_RISCV_TLS_DTPMOD32)	\
     || (__WORDSIZE == 32 && (type) == R_RISCV_TLS_TPREL32)	\
     || (__WORDSIZE == 64 && (type) == R_RISCV_TLS_DTPREL64)	\
     || (__WORDSIZE == 64 && (type) == R_RISCV_TLS_DTPMOD64)	\
     || (__WORDSIZE == 64 && (type) == R_RISCV_TLS_TPREL64)))	\
   | (ELF_RTYPE_CLASS_COPY * ((type) == R_RISCV_COPY)))

#define ELF_MACHINE_NO_REL 1
#define ELF_MACHINE_NO_RELA 0

/* Return nonzero iff ELF header is compatible with the running host.  */
static inline int __attribute_used__
elf_machine_matches_host (const ElfW(Ehdr) *ehdr)
{
  /* We can only run RISC-V binaries.  */
  if (ehdr->e_machine != EM_RISCV)
    return 0;

  /* Ensure the library's floating-point ABI matches that of the running
     system.  For now we don't support mixing XLEN, so there's no need (or way)
     to check it matches.  */
#ifdef __riscv_float_abi_double
  if ((ehdr->e_flags & EF_RISCV_FLOAT_ABI) != EF_RISCV_FLOAT_ABI_DOUBLE)
    return 0;
#else
  if ((ehdr->e_flags & EF_RISCV_FLOAT_ABI) != EF_RISCV_FLOAT_ABI_SOFT)
    return 0;
#endif

  return 1;
}

/* Return the link-time address of _DYNAMIC.  */
static inline ElfW(Addr) __attribute__((always_inline))
elf_machine_dynamic (void)
{
#ifndef __CHERI_PURE_CAPABILITY__
  extern ElfW(Addr) _GLOBAL_OFFSET_TABLE_ __attribute__ ((visibility ("hidden")));
  return _GLOBAL_OFFSET_TABLE_;
#else /* __CHERI_PURE_CAPABILITY__ */
  /* Globals are not initialized yet when this function is used
     therefore, access symbol using PC-relative addressing */
  ElfW(Addr) *got;
  asm ("cllc %0, _GLOBAL_OFFSET_TABLE_" : "=C" (got));
  return *got;
#endif /* __CHERI_PURE_CAPABILITY__ */
}

#define STRINGXP(X) __STRING (X)
#define STRINGXV(X) STRINGV_ (X)
#define STRINGV_(...) # __VA_ARGS__

/* Return the run-time load address of the shared object.  */
static inline ElfW(Addr) __attribute__((always_inline))
elf_machine_load_address (void)
{
  
#ifndef __CHERI_PURE_CAPABILITY__
  ElfW(Addr) load_addr;
  asm ("lla %0, _DYNAMIC" : "=r" (load_addr));
#else
	/* cheri TODO: review quick fix uintptr_t w.r.t. usage of ELf64_Addr (potential cast to ptr/cap)*/ 
  uintptr_t load_addr;
  asm ("cllc %0, _DYNAMIC" : "=C" (load_addr));
#endif
  return load_addr - elf_machine_dynamic ();
}

/* Initial entry point code for the dynamic linker.
   The C function `_dl_start' is the real entry point;
   its return value is the user program's entry point.  */
#ifndef __CHERI_PURE_CAPABILITY__
#define RTLD_START asm (\
	".text\n\
	" _RTLD_PROLOGUE (ENTRY_POINT) "\
	mv a0, sp\n\
	jal _dl_start\n\
	# Stash user entry point in s0.\n\
	mv s0, a0\n\
	# See if we were run as a command with the executable file\n\
	# name as an extra leading argument.\n\
	lw a0, _dl_skip_args\n\
	# Load the original argument count.\n\
	" STRINGXP (REG_L) " a1, 0(sp)\n\
	# Subtract _dl_skip_args from it.\n\
	sub a1, a1, a0\n\
	# Adjust the stack pointer to skip _dl_skip_args words.\n\
	sll a0, a0, " STRINGXP (PTRLOG) "\n\
	add sp, sp, a0\n\
	# Save back the modified argument count.\n\
	" STRINGXP (REG_S) " a1, 0(sp)\n\
	# Call _dl_init (struct link_map *main_map, int argc, char **argv, char **env) \n\
	" STRINGXP (REG_L) " a0, _rtld_local\n\
	add a2, sp, " STRINGXP (SZREG) "\n\
	sll a3, a1, " STRINGXP (PTRLOG) "\n\
	add a3, a3, a2\n\
	add a3, a3, " STRINGXP (SZREG) "\n\
	# Call the function to run the initializers.\n\
	jal _dl_init\n\
	# Pass our finalizer function to _start.\n\
	lla a0, _dl_fini\n\
	# Jump to the user entry point.\n\
	jr s0\n\
	" _RTLD_EPILOGUE (ENTRY_POINT) "\
	.previous" \
);

#else /* __CHERI_PURE_CAPABILITY__ */
#define RTLD_START asm (\
	".text\n\
	" _RTLD_PROLOGUE (ENTRY_POINT) "\
	cmove ca0, csp\n\
	cllc ct0, _dl_start\n\
	cjalr ct0 \n\
	# Stash user entry point in s0.\n\
  auipcc cs0, 0\n\
  csetaddr cs0, cs0, a0\n\
	# ca0 is not a capability cmove cs0, ca0\n\
	# See if we were run as a command with the executable file\n\
	# name as an extra leading argument.\n\
	1: auipcc ca0, %pcrel_hi(_dl_skip_args)\n\
  	clw  a0, %pcrel_lo(1b)(ca0)\n\
	# Load the original argument count.\n\
	" STRINGXP (REG_L) " a1, 0(csp)\n\
	# Subtract _dl_skip_args from it.\n\
	sub a1, a1, a0\n\
	# Adjust the stack pointer to skip _dl_skip_args words.\n\
	sll a0, a0, " STRINGXP (PTRLOG) "\n\
	cincoffset csp, csp, a0 \n\
	# Save back the modified argument count.\n\
	" STRINGXP (REG_S) " a1, 0(csp)\n\
	# Call _dl_init (struct link_map *main_map, int argc, char **argv, char **env) \n\
	cllc ca0, _rtld_local\n\
	clc ca0, (ca0)\n\
	cincoffset ca2, csp, " STRINGXP (SZCREG) "\n\
	sll a3, a1, " STRINGXP (PTRLOG) "\n\
	cincoffset ca3, ca2, a3\n\
	# TODO cheri: skipping the NULL in argv? \n\
	cincoffset ca3, ca3, " STRINGXP (SZCREG) "\n\
	# Call the function to run the initializers.\n\
  	cllc ct0, _dl_init\n\
	cjalr ct0 \n\
	# Pass our finalizer function to _start.\n\
	cllc ca0, _dl_fini\n\
	# Jump to the user entry point.\n\
	cjr cs0\n\
	" _RTLD_EPILOGUE (ENTRY_POINT) "\
	.previous" \
);
#endif /* __CHERI_PURE_CAPABILITY__ */

/* Names of the architecture-specific auditing callback functions.  */
#define ARCH_LA_PLTENTER riscv_gnu_pltenter
#define ARCH_LA_PLTEXIT riscv_gnu_pltexit

/* Bias .got.plt entry by the offset requested by the PLT header.  */
#define elf_machine_plt_value(map, reloc, value) (value)

static inline ElfW(Addr)
elf_machine_fixup_plt (struct link_map *map, lookup_t t,
		       const ElfW(Sym) *refsym, const ElfW(Sym) *sym,
		       const ElfW(Rela) *reloc,
		       ElfW(Addr) *reloc_addr, ElfW(Addr) value)
{
  return *reloc_addr = value;
}

#endif /* !dl_machine_h */

#ifdef RESOLVE_MAP

/* Perform a relocation described by R_INFO at the location pointed to
   by RELOC_ADDR.  SYM is the relocation symbol specified by R_INFO and
   MAP is the object containing the reloc.  */

auto inline void
__attribute__ ((always_inline))
elf_machine_rela (struct link_map *map, const ElfW(Rela) *reloc,
		  const ElfW(Sym) *sym, const struct r_found_version *version,
		  void *const reloc_addr, int skip_ifunc
#ifndef NESTING
		  , struct link_map *boot_map
#endif
		  )
{
  ElfW(Addr) r_info = reloc->r_info;
  const unsigned long int r_type = ELFW (R_TYPE) (r_info);
  ElfW(Addr) *addr_field = (ElfW(Addr) *) reloc_addr;
  const ElfW(Sym) *const __attribute__ ((unused)) refsym = sym;
#if !defined NESTING && (defined RTLD_BOOTSTRAP || defined STATIC_PIE_BOOTSTRAP)
  struct link_map *sym_map = boot_map;
#else
  struct link_map *sym_map = RESOLVE_MAP (&sym, version, r_type);
#endif
  ElfW(Addr) value = 0;
  if (sym_map != NULL)
    value = sym_map->l_addr + sym->st_value + reloc->r_addend;

  switch (r_type)
    {
#ifndef RTLD_BOOTSTRAP
    case __WORDSIZE == 64 ? R_RISCV_TLS_DTPMOD64 : R_RISCV_TLS_DTPMOD32:
      if (sym_map)
	*addr_field = sym_map->l_tls_modid;
      break;

    case __WORDSIZE == 64 ? R_RISCV_TLS_DTPREL64 : R_RISCV_TLS_DTPREL32:
      if (sym != NULL)
	*addr_field = TLS_DTPREL_VALUE (sym) + reloc->r_addend;
      break;

    case __WORDSIZE == 64 ? R_RISCV_TLS_TPREL64 : R_RISCV_TLS_TPREL32:
      if (sym != NULL)
	{
	  CHECK_STATIC_TLS (map, sym_map);
	  *addr_field = TLS_TPREL_VALUE (sym_map, sym) + reloc->r_addend;
	}
      break;

    case R_RISCV_COPY:
      {
	if (__glibc_unlikely (sym == NULL))
	  /* This can happen in trace mode if an object could not be
	     found.  */
	  break;

	/* Handle TLS copy relocations.  */
	if (__glibc_unlikely (ELFW (ST_TYPE) (sym->st_info) == STT_TLS))
	  {
	    /* There's nothing to do if the symbol is in .tbss.  */
	    if (__glibc_likely (sym->st_value >= sym_map->l_tls_initimage_size))
	      break;
	    value += (ElfW(Addr)) sym_map->l_tls_initimage - sym_map->l_addr;
	  }

	size_t size = sym->st_size;
	if (__glibc_unlikely (sym->st_size != refsym->st_size))
	  {
	    const char *strtab = (const void *) CHERI_CAST(D_PTR (map, l_info[DT_STRTAB]), map->l_info[DT_STRSZ]->d_un.d_val);
	    if (sym->st_size > refsym->st_size)
	      size = refsym->st_size;
	    if (sym->st_size > refsym->st_size || GLRO(dl_verbose))
	      _dl_error_printf ("\
  %s: Symbol `%s' has different size in shared object, consider re-linking\n",
				rtld_progname ?: "<program name unknown>",
				strtab + refsym->st_name);
	  }

	memcpy (reloc_addr, (void *)CHERI_CAST(value, -1), size);
	break;
      }
#endif

#if !defined RTLD_BOOTSTRAP || !defined HAVE_Z_COMBRELOC
    case R_RISCV_RELATIVE:
      {
# if !defined RTLD_BOOTSTRAP && !defined HAVE_Z_COMBRELOC
	/* This is defined in rtld.c, but nowhere in the static libc.a;
	   make the reference weak so static programs can still link.
	   This declaration cannot be done when compiling rtld.c
	   (i.e. #ifdef RTLD_BOOTSTRAP) because rtld.c contains the
	   common defn for _dl_rtld_map, which is incompatible with a
	   weak decl in the same file.  */
#  ifndef SHARED
	weak_extern (GL(dl_rtld_map));
#  endif
	if (map != &GL(dl_rtld_map)) /* Already done in rtld itself.  */
# endif
	  *addr_field = map->l_addr + reloc->r_addend;
      break;
    }
#endif

    case R_RISCV_JUMP_SLOT:
    case __WORDSIZE == 64 ? R_RISCV_64 : R_RISCV_32:
      *addr_field = value;
      break;

    case R_RISCV_NONE:
      break;

#ifdef __CHERI_PURE_CAPABILITY__
	case R_RISCV_CHERI_CAPABILITY:
	  // sym_val_p ~> (sym_map->l_addr + sym->st_value) type: (void*) in musl
	  // in musl: *(void**) reloc_addr = sym_val_p
	  if (sym != NULL) {
		  if (ELFW(ST_TYPE) (sym->st_info) == STT_FUNC) {
			// TODO Cheri depending on ABI set tight bounds for functions
			// TODO Cheri make function sentry capability
			*(void**) addr_field =  __builtin_cheri_seal_entry((void*) CHERI_FN_CAST(sym_map->l_addr + sym->st_value, -1));
		  } else {
			*(void**) addr_field =  (void*) CHERI_CAST(sym_map->l_addr+ sym->st_value, sym->st_size);
		  }

	  }
	  break;
#endif
    default:
      _dl_reloc_bad_type (map, r_type, 0);
      break;
    }
}

auto inline void
__attribute__ ((always_inline))
elf_machine_rela_relative (ElfW(Addr) l_addr, const ElfW(Rela) *reloc,
			  void *const reloc_addr)
{
  *(ElfW(Addr) *) reloc_addr = l_addr + reloc->r_addend;
}

auto inline void
__attribute__ ((always_inline))
elf_machine_lazy_rel (struct link_map *map, ElfW(Addr) l_addr,
		      const ElfW(Rela) *reloc, int skip_ifunc)
{
  ElfW(Addr) *const reloc_addr = (void *) (CHERI_CAST(l_addr + reloc->r_offset, -1));
  const unsigned int r_type = ELFW (R_TYPE) (reloc->r_info);

  /* Check for unexpected PLT reloc type.  */
  if (__glibc_likely (r_type == R_RISCV_JUMP_SLOT))
    {
      if (__glibc_unlikely (map->l_mach.plt == 0))
	{
	  if (l_addr)
	    *reloc_addr += l_addr;
	}
      else
	*reloc_addr = map->l_mach.plt;
    }
  else
    _dl_reloc_bad_type (map, r_type, 1);
}

/* Set up the loaded object described by L so its stub function
   will jump to the on-demand fixup code __dl_runtime_resolve.  */

auto inline int
__attribute__ ((always_inline))
elf_machine_runtime_setup (struct link_map *l, int lazy, int profile)
{
#ifndef RTLD_BOOTSTRAP
  /* If using PLTs, fill in the first two entries of .got.plt.  */
  if (l->l_info[DT_JMPREL])
    {
      extern void _dl_runtime_resolve (void) __attribute__ ((visibility ("hidden")));
      ElfW(Addr) *gotplt = (ElfW(Addr) *) CHERI_CAST(D_PTR (l, l_info[DT_PLTGOT]), -1);
      /* If a library is prelinked but we have to relocate anyway,
	 we have to be able to undo the prelinking of .got.plt.
	 The prelinker saved the address of .plt for us here.  */
      if (gotplt[1])
	l->l_mach.plt = gotplt[1] + l->l_addr;
      gotplt[0] = (ElfW(Addr)) &_dl_runtime_resolve;
      gotplt[1] = (ElfW(Addr)) l;
    }
#endif

  return lazy;
}

#endif /* RESOLVE_MAP */
