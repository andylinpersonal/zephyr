/*
 * Copyright (c) 2025 Andy Lin
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SOC_ARM_RP2350_SOC_CONTEXT_H_
#define SOC_ARM_RP2350_SOC_CONTEXT_H_

#include <zephyr/toolchain.h>

#if defined(CONFIG_ARM_SOC_CONTEXT_SAVE)

#if defined(CONFIG_PICOSDK_USE_DCP_COPROC)
#define SOC_ESF_MEMBERS                \
	uint32_t dcp_xml;              \
	uint32_t dcp_xmh;              \
	uint32_t dcp_yml;              \
	uint32_t dcp_ymh;              \
	uint32_t dcp_exp_flag_statusl; \
	uint32_t dcp_exp_flag_statush

#define SOC_ESF_INIT 0, 0, 0, 0, 0, 0
#else
#define SOC_ESF_MEMBERS uint32_t dummy
#define SOC_ESF_INIT    0
#endif

#endif /* CONFIG_ARM_SOC_CONTEXT_SAVE */

#endif /* SOC_ARM_RP2040_SOC_CONTEXT_H_ */
