/*
 * Copyright (c) 2025 Andy Lin
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/offsets.h>
#include <zephyr/toolchain.h>
#include <zephyr/arch/arm/cortex_m/cpu.h>

#if defined(CONFIG_PICOSDK_USE_DCP_COPROC)
#include <hardware/dcp_instr.inc.S>
#include <hardware/dcp_canned.inc.S>
#endif

_ASM_FILE_PROLOGUE

#if defined(CONFIG_ARM_SOC_CONTEXT_SAVE)

/* Exports */
GTEXT(__soc_save_context)
GTEXT(__soc_restore_context)

SECTION_FUNC(TEXT, __soc_save_context)
#if defined(CONFIG_PICOSDK_USE_DCP_COPROC)
	push {r4-r5}
	nop
	PXMD r1, r2
	PYMD r3, r4
	REFD r5, ip
	stmia r0!, {r1-r5,ip}
	pop {r4-r5}
#endif
	bx lr

SECTION_FUNC(TEXT, __soc_restore_context)
#if defined(CONFIG_PICOSDK_USE_DCP_COPROC)
	push {r4-r5}
	nop
	ldmia r0!, {r1-r5,ip}
	WXMD r1, r2
	WYMD r3, r4
	WEFD r5, ip
	pop {r4-r5}
#endif
	bx lr

#endif /* CONFIG_ARM_SOC_CONTEXT_SAVE */
