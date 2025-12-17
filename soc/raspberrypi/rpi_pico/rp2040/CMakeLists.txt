# Copyright (c) 2021 Yonatan Schachter
# SPDX-License-Identifier: Apache-2.0

zephyr_library()

zephyr_include_directories(.)

zephyr_library_sources(
    soc.c
)
zephyr_library_sources_ifdef(CONFIG_ARM_MPU
    mpu_regions.c
)

zephyr_library_sources_ifdef(CONFIG_ARM_SOC_CONTEXT_SAVE
    soc_irq.S
    soc_dump_context.c
)

set(SOC_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld CACHE INTERNAL "")
