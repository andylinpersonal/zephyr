/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Align with Zephyr's convention */
#define RAM_SECTION_NAME(x) .ramfunc.##x

#include "pico/asm_helper.S"
#include "hardware/regs/addressmap.h"
#include "hardware/divider_helper.S"

pico_default_asm_setup

.macro div_section name
#if CONFIG_PICOSDK_DIVIDER_IN_RAM
.section RAM_SECTION_NAME(\name), "ax"
#else
.section SECTION_NAME(\name), "ax"
#endif
.endm

GTEXT(divmod_s32s32_unsafe_bad)
/* These trampolines are workaround for uninitialized SIO_BASE in picosdk's
 * implementation of __aeabi_idiv and __aeabi_uidiv. */
div_section WRAPPER_FUNC_NAME(__aeabi_idiv)
.align 2

wrapper_func __aeabi_idiv
wrapper_func __aeabi_idivmod
regular_func divmod_s32s32_unsafe
	/* ldr r0, =SIO_BASE (0xD0000000) */
	movs	r2, #0xd
	lsls	r2, #28
	/* r3 is unused and caller-save so we can clobber it for free */
	ldr	r3, =divmod_s32s32_unsafe_bad
	bx	r3

GTEXT(divmod_u32u32_unsafe_bad)
div_section WRAPPER_FUNC_NAME(__aeabi_uidiv)
.align 2

wrapper_func __aeabi_uidiv
wrapper_func __aeabi_uidivmod
regular_func divmod_u32u32_unsafe
	movs	r2, #0xd
	lsls	r2, #28
	ldr	r3, =divmod_u32u32_unsafe_bad
	bx	r3

div_section WRAPPER_FUNC_NAME(__aeabi_ldiv)
.align 2

GTEXT(divmod_s64s64_unsafe)
wrapper_func __aeabi_ldivmod
	push	{r4}
	ldr	r4, =divmod_s64s64_unsafe
	mov	ip, r4
	pop	{r4}
	bx	ip

GTEXT(divmod_u64u64_unsafe)
wrapper_func __aeabi_uldivmod
	push	{r4}
	ldr	r4, =divmod_u64u64_unsafe
	mov	ip, r4
	pop	{r4}
	bx	ip
