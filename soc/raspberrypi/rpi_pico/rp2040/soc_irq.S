/*
 * Copyright (c) 2025 Andy Lin
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/offsets.h>
#include <zephyr/toolchain.h>
#include <zephyr/arch/arm/cortex_m/cpu.h>

#include <hardware/regs/addressmap.h>
#include <hardware/regs/sio.h>

_ASM_FILE_PROLOGUE

#if defined(CONFIG_ARM_SOC_CONTEXT_SAVE)

/* Exports */
GTEXT(__soc_save_context)
GTEXT(__soc_restore_context)

SECTION_FUNC(TEXT, __soc_save_context)
#if defined(CONFIG_RP2_USE_SIO_DIVIDER)
	push	{r4,r5}
	movs	r1, #0xd
	lsls	r1, #28 /* ldr r1, =SIO_BASE (0xD0000000) */
	ldr	r2, [r1, #SIO_DIV_UDIVIDEND_OFFSET]
	ldr	r3, [r1, #SIO_DIV_UDIVISOR_OFFSET]
	ldr	r4, [r1, #SIO_DIV_REMAINDER_OFFSET]
	ldr	r5, [r1, #SIO_DIV_QUOTIENT_OFFSET]
	stmia	r0!, {r2-r5}
	pop	{r4,r5}
#endif
	bx	lr

SECTION_FUNC(TEXT, __soc_restore_context)
#if defined(CONFIG_RP2_USE_SIO_DIVIDER)
	push	{r4}
	ldmia	r0!, {r1-r4}
	movs	r0, #0xd
	lsls	r0, #28 /* ldr r0, =SIO_BASE (0xD0000000) */
	str	r1, [r0, #SIO_DIV_UDIVIDEND_OFFSET]
	str	r2, [r0, #SIO_DIV_UDIVISOR_OFFSET]
	str	r3, [r0, #SIO_DIV_REMAINDER_OFFSET]
	str	r4, [r0, #SIO_DIV_QUOTIENT_OFFSET]
	pop	{r4}
#endif
	bx	lr

#endif /* CONFIG_ARM_SOC_CONTEXT_SAVE */
