/**
 * @file
 * @brief System/hardware module for Raspberry Pi RP2040 MCU
 *
 * This module provides routines to initialize and support board-level hardware
 * for the Raspberry Pi RP2040
 */
#include <zephyr/linker/section_tags.h>
#include <zephyr/init.h>
#include <sys/cdefs.h>
#include <stddef.h>

#if IS_ENABLED(CONFIG_PICOSDK_USE_ROM_MEMOPS)
/* Minimal implementation before the ROM function table being initialized. */
__boot_func __no_builtin void arch_early_memset(void *dst, int c, size_t n)
{
	extern void *__real_memset(void *dst, int c, size_t n);
	(void)__real_memset(dst, c, n);
}

__boot_func __no_builtin void arch_early_memcpy(void *dst, const void *src, size_t n)
{
	extern void *__real_memcpy(void *dst, const void *src0, size_t n);
	(void)__real_memcpy(dst, src, n);
}
#endif
