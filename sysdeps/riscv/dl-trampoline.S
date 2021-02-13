/* RISC-V PLT trampoline
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
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <sysdep.h>
#include <sys/asm.h>

/* Assembler veneer called from the PLT header code for lazy loading.
   The PLT header passes its own args in t0-t2.  */

#ifndef __CHERI_PURE_CAPABILITY__
#ifdef __riscv_float_abi_soft
# define FRAME_SIZE (-((-10 * SZREG) & ALMASK))
#else
# define FRAME_SIZE (-((-10 * SZREG - 8 * SZFREG) & ALMASK))
#endif
#else /* __CHERI_PURE_CAPABILITY__ */
#ifdef __riscv_float_abi_soft
# define FRAME_SIZE (-((-10 * SZCREG) & ALMASK))
#else
# define FRAME_SIZE (-((-10 * SZCREG - 8 * SZFREG) & ALMASK))
#endif
#endif /* __CHERI_PURE_CAPABILITY__ */

ENTRY (_dl_runtime_resolve)
#ifndef __CHERI_PURE_CAPABILITY__
  # Save arguments to stack.
  addi sp, sp, -FRAME_SIZE
  REG_S ra, 9*SZREG(sp)
  REG_S a0, 1*SZREG(sp)
  REG_S a1, 2*SZREG(sp)
  REG_S a2, 3*SZREG(sp)
  REG_S a3, 4*SZREG(sp)
  REG_S a4, 5*SZREG(sp)
  REG_S a5, 6*SZREG(sp)
  REG_S a6, 7*SZREG(sp)
  REG_S a7, 8*SZREG(sp)

#ifndef __riscv_float_abi_soft
  FREG_S fa0, (10*SZREG + 0*SZFREG)(sp)
  FREG_S fa1, (10*SZREG + 1*SZFREG)(sp)
  FREG_S fa2, (10*SZREG + 2*SZFREG)(sp)
  FREG_S fa3, (10*SZREG + 3*SZFREG)(sp)
  FREG_S fa4, (10*SZREG + 4*SZFREG)(sp)
  FREG_S fa5, (10*SZREG + 5*SZFREG)(sp)
  FREG_S fa6, (10*SZREG + 6*SZFREG)(sp)
  FREG_S fa7, (10*SZREG + 7*SZFREG)(sp)
#endif

  # Update .got.plt and obtain runtime address of callee.
  slli a1, t1, 1
  mv a0, t0       # link map
  add a1, a1, t1  # reloc offset (== thrice the .got.plt offset)
  la a2, _dl_fixup
  jalr a2
  mv t1, a0

  # Restore arguments from stack.
  REG_L ra, 9*SZREG(sp)
  REG_L a0, 1*SZREG(sp)
  REG_L a1, 2*SZREG(sp)
  REG_L a2, 3*SZREG(sp)
  REG_L a3, 4*SZREG(sp)
  REG_L a4, 5*SZREG(sp)
  REG_L a5, 6*SZREG(sp)
  REG_L a6, 7*SZREG(sp)
  REG_L a7, 8*SZREG(sp)

#ifndef __riscv_float_abi_soft
  FREG_L fa0, (10*SZREG + 0*SZFREG)(sp)
  FREG_L fa1, (10*SZREG + 1*SZFREG)(sp)
  FREG_L fa2, (10*SZREG + 2*SZFREG)(sp)
  FREG_L fa3, (10*SZREG + 3*SZFREG)(sp)
  FREG_L fa4, (10*SZREG + 4*SZFREG)(sp)
  FREG_L fa5, (10*SZREG + 5*SZFREG)(sp)
  FREG_L fa6, (10*SZREG + 6*SZFREG)(sp)
  FREG_L fa7, (10*SZREG + 7*SZFREG)(sp)
#endif

  addi sp, sp, FRAME_SIZE

  # Invoke the callee.
  jr t1

#else /* __CHERI_PURE_CAPABILITY__ */
  # Save arguments to stack.
  cincoffset csp, csp, -FRAME_SIZE
  CREG_S cra, 9*SZREG(csp)
  CREG_S ca0, 1*SZREG(csp)
  CREG_S ca1, 2*SZREG(csp)
  CREG_S ca2, 3*SZREG(csp)
  CREG_S ca3, 4*SZREG(csp)
  CREG_S ca4, 5*SZREG(csp)
  CREG_S ca5, 6*SZREG(csp)
  CREG_S ca6, 7*SZREG(csp)
  CREG_S ca7, 8*SZREG(csp)

#ifndef __riscv_float_abi_soft
  FREG_S fa0, (10*SZCREG + 0*SZFREG)(csp)
  FREG_S fa1, (10*SZCREG + 1*SZFREG)(csp)
  FREG_S fa2, (10*SZCREG + 2*SZFREG)(csp)
  FREG_S fa3, (10*SZCREG + 3*SZFREG)(csp)
  FREG_S fa4, (10*SZCREG + 4*SZFREG)(csp)
  FREG_S fa5, (10*SZCREG + 5*SZFREG)(csp)
  FREG_S fa6, (10*SZCREG + 6*SZFREG)(csp)
  FREG_S fa7, (10*SZCREG + 7*SZFREG)(csp)
#endif

  # Update .got.plt and obtain runtime address of callee.
  slli a1, t1, 1
  cmove ca0, ct0       # link map
  add a1, a1, t1  # reloc offset (== thrice the .got.plt offset)
  cllc ca2, _dl_fixup
  cjalr ca2
  cmove ct1, ca0

  # Restore arguments from stack.
  CREG_L cra, 9*SZCREG(csp)
  CREG_L ca0, 1*SZCREG(csp)
  CREG_L ca1, 2*SZCREG(csp)
  CREG_L ca2, 3*SZCREG(csp)
  CREG_L ca3, 4*SZCREG(csp)
  CREG_L ca4, 5*SZCREG(csp)
  CREG_L ca5, 6*SZCREG(csp)
  CREG_L ca6, 7*SZCREG(csp)
  CREG_L ca7, 8*SZCREG(csp)

#ifndef __riscv_float_abi_soft
  FREG_L fa0, (10*SZCREG + 0*SZFREG)(csp)
  FREG_L fa1, (10*SZCREG + 1*SZFREG)(csp)
  FREG_L fa2, (10*SZCREG + 2*SZFREG)(csp)
  FREG_L fa3, (10*SZCREG + 3*SZFREG)(csp)
  FREG_L fa4, (10*SZCREG + 4*SZFREG)(csp)
  FREG_L fa5, (10*SZCREG + 5*SZFREG)(csp)
  FREG_L fa6, (10*SZCREG + 6*SZFREG)(csp)
  FREG_L fa7, (10*SZCREG + 7*SZFREG)(csp)
#endif

  cincoffset csp, csp, FRAME_SIZE

  # Invoke the callee.
  cjr ct1

#endif /* __CHERI_PURE_CAPABILITY__ */
END (_dl_runtime_resolve)
