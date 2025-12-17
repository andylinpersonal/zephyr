/*
 * Copyright (c) 2024 Andrew Featherstone
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief System/hardware module for Raspberry Pi RP235xx MCUs
 *
 * This module provides routines to initialize and support board-level hardware
 * for the Raspberry Pi RP235xx (RP2350A, RP2350B, RP2354A, RP2354B).
 */

#include <pico/bootrom.h>
#include <boot/picoboot.h>
#include <zephyr/kernel.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/sys/util.h>

#if CONFIG_SOC_RESET_HOOK
#include <pico/runtime_init.h>
#if CONFIG_RISCV
#include <hardware/riscv_platform_timer.h>
#endif

#if IS_ENABLED(CONFIG_CPU_CORTEX_M)
#include <hardware/gpio.h> /* PICO_USE_GPIO_COPROCESSOR is defined here */
#include <hardware/structs/m33.h>

void runtime_init_per_core_enable_coprocessors(void)
{
	uint32_t cpacr = 0;
#if IS_ENABLED(CONFIG_FPU)
	/* VFP copro (float) */
	cpacr |= M33_CPACR_CP10_BITS;
#endif
#if HAS_DOUBLE_COPROCESSOR && IS_ENABLED(CONFIG_PICOSDK_USE_DCP_COPROC)
	cpacr |= M33_CPACR_CP4_BITS;
#endif
#if PICO_USE_GPIO_COPROCESSOR
	cpacr |= M33_CPACR_CP0_BITS;
#endif
	arm_cpu_hw->cpacr |= cpacr;
#if HAS_DOUBLE_COPROCESSOR && IS_ENABLED(CONFIG_PICOSDK_USE_DCP_COPROC)
	asm volatile("mrc p4,#0,r0,c0,c0,#1" : : : "r0"); /* clear engaged flag via RCMP */
#endif
}
#endif /* CONFIG_CPU_CORTEX_M */

void soc_reset_hook(void)
{
#if CONFIG_RISCV
	/* The kernel requires the mtimer to tick at the CPU frequency. */
	riscv_timer_set_fullspeed(true);
#else
	runtime_init_per_core_enable_coprocessors();
#endif
}

#endif /* CONFIG_SOC_RESET_HOOK */

#if CONFIG_RISCV
void sys_arch_reboot(int type)
{
	ARG_UNUSED(type);
	/* Cannot be 0 ms */
	rom_reboot(REBOOT2_FLAG_REBOOT_TYPE_NORMAL | REBOOT2_FLAG_NO_RETURN_ON_SUCCESS, 1, 0, 0);
}
#endif
