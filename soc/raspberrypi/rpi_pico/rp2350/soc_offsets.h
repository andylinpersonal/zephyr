/*
 * Copyright (c) 2025 Andy Lin
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SOC_ARM_RP2040_SOC_OFFSETS_H_
#define SOC_ARM_RP2040_SOC_OFFSETS_H_

#if defined(CONFIG_ARM_SOC_OFFSETS)

#if defined(CONFIG_PICOSDK_USE_DCP_COPROC)
#define GEN_SOC_OFFSET_SYMS()                            \
	GEN_OFFSET_SYM(soc_esf_t, dcp_xml);              \
	GEN_OFFSET_SYM(soc_esf_t, dcp_xmh);              \
	GEN_OFFSET_SYM(soc_esf_t, dcp_yml);              \
	GEN_OFFSET_SYM(soc_esf_t, dcp_ymh);              \
	GEN_OFFSET_SYM(soc_esf_t, dcp_exp_flag_statusl); \
	GEN_OFFSET_SYM(soc_esf_t, dcp_exp_flag_statush)
#else
#define GEN_SOC_OFFSET_SYMS()
#endif

#endif /* CONFIG_ARM_SOC_OFFSETS */

#endif /* SOC_ARM_RP2040_SOC_OFFSETS_H_ */
