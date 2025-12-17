/*
 * Copyright (c) 2025 Andy Lin <andylinpersonal@gmail.com>.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <hardware/regs/addressmap.h>

#include <zephyr/arch/arm/mpu/arm_mpu_mem_cfg.h>

#define RP2040_BOOTROM_REGION_SIZE REGION_16K

#if IS_ENABLED(CONFIG_RP2_USERSPACE_SIO)
#define RP2040_SIO_NO_SPINLOCK_REGION_SIZE REGION_256B

/* Expected to be covered by corresponding drivers and kernel. */
#define RP2040_MPU_SUBREGION_SIO_CPUID_AND_GPIO_LOW SUB_REGION_0_DISABLED
#define RP2040_MPU_SUBREGION_SIO_GPIO_MID           SUB_REGION_1_DISABLED
#define RP2040_MPU_SUBREGION_SIO_GPIO_HIGH_AND_FIFO SUB_REGION_2_DISABLED

/* Hardware divider is located at offset 0x60, occupying subregion 3 in a 256B region */
#define RP2040_MPU_SUBREGION_SIO_DIVIDER                                                           \
	(COND_CODE_1(CONFIG_RP2_USERSPACE_DIVIDER, (0), SUB_REGION_3_DISABLED))

/* Interpolator 0 is located at offset 0x80, occupying subregion 4 and 5 in a 256B region */
#define RP2040_MPU_SUBREGION_INTERP0_LOW  SUB_REGION_4_DISABLED
#define RP2040_MPU_SUBREGION_INTERP0_HIGH SUB_REGION_5_DISABLED

/* Interpolator 1 is located at offset 0xc0, occupying subregion 6 and 7 in a 256B region */
#define RP2040_MPU_SUBREGION_INTERP1_LOW  SUB_REGION_6_DISABLED
#define RP2040_MPU_SUBREGION_INTERP1_HIGH SUB_REGION_7_DISABLED

#define RP2040_SIO_NO_SPINLOCKS_MPU_ATTR                                                           \
	{(DEVICE_NON_SHAREABLE | RP2040_SIO_NO_SPINLOCK_REGION_SIZE | P_RW_U_RW_Msk | NOT_EXEC) |  \
	 (RP2040_MPU_SUBREGION_SIO_CPUID_AND_GPIO_LOW | RP2040_MPU_SUBREGION_SIO_GPIO_MID |        \
	  RP2040_MPU_SUBREGION_SIO_GPIO_HIGH_AND_FIFO | RP2040_MPU_SUBREGION_SIO_DIVIDER |         \
	  RP2040_MPU_SUBREGION_INTERP0_LOW | RP2040_MPU_SUBREGION_INTERP0_HIGH |                   \
	  RP2040_MPU_SUBREGION_INTERP1_LOW | RP2040_MPU_SUBREGION_INTERP1_HIGH)}
#endif /* CONFIG_RP2_USERSPACE_SIO */

static const struct arm_mpu_region mpu_regions[] = {
#if IS_ENABLED(CONFIG_RPI_PICO_USE_ROMFUNC)
	MPU_REGION_ENTRY("BOOTROM", ROM_BASE, REGION_FLASH_ATTR(RP2040_BOOTROM_REGION_SIZE)),
#endif

#if IS_ENABLED(CONFIG_XIP)
	MPU_REGION_ENTRY("FLASH", CONFIG_FLASH_BASE_ADDRESS, REGION_FLASH_ATTR(REGION_FLASH_SIZE)),
#endif

	MPU_REGION_ENTRY("SRAM_0", CONFIG_SRAM_BASE_ADDRESS, REGION_RAM_ATTR(REGION_SRAM_SIZE)),

#if IS_ENABLED(CONFIG_RP2_USERSPACE_SIO)
	/* Region 3 */
	MPU_REGION_ENTRY("SIO_NO_SPINLOCK", SIO_BASE, RP2040_SIO_NO_SPINLOCKS_MPU_ATTR),
#endif
};

const struct arm_mpu_config mpu_config = {
	.num_regions = ARRAY_SIZE(mpu_regions),
	.mpu_regions = mpu_regions,
};
