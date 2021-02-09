/* Miscellaneous macros.
   Copyright (C) 2000-2018 Free Software Foundation, Inc.
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

#ifndef _SYS_ASM_H
#define _SYS_ASM_H

/* Macros to handle different pointer/register sizes for 32/64-bit code.  */
#if __riscv_xlen == 64
# define SZREG	8
#ifndef __CHERI_PURE_CAPABILITY__
# define PTRLOG 3
# define REG_S sd
# define REG_L ld
#else
# define PTRLOG 4
# define REG_S csd
# define REG_L cld
# define CREG_S csc
# define CREG_L clc
# define SZCREG	16
#endif
#elif __riscv_xlen == 32
# error "rv32i-based targets are not supported"
#else
# error __riscv_xlen must equal 32 or 64
#endif

#if !defined __riscv_float_abi_soft
/* For ABI uniformity, reserve 8 bytes for floats, even if double-precision
   floating-point is not supported in hardware.  */
# if defined __riscv_float_abi_double
#  define SZFREG 8
#ifndef __CHERI_PURE_CAPABILITY__
#  define FREG_L fld
#  define FREG_S fsd
#else
# define FREG_S cfsd
# define FREG_L cfld
#endif
# else
#  error unsupported FLEN
# endif
#endif

/* Declare leaf routine.  */
#define	LEAF(symbol)				\
		.globl	symbol;			\
		.align	2;			\
		.type	symbol,@function;	\
symbol:						\
		cfi_startproc;

/* Mark end of function.  */
#undef END
#define END(function)				\
		cfi_endproc;			\
		.size	function,.-function

/* Stack alignment.  */
#ifndef __CHERI_PURE_CAPABILITY__
#define ALMASK	~15
#else
#define ALMASK ~31
#endif

#endif /* sys/asm.h */
