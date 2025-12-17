/*
 * Copyright (c) 2025 Andy Lin
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SOC_ARM_RP2040_SOC_OFFSETS_H_
#define SOC_ARM_RP2040_SOC_OFFSETS_H_

#ifdef CONFIG_ARM_SOC_OFFSETS

#if defined(CONFIG_RP2_USE_SIO_DIVIDER)
#define GEN_SOC_OFFSET_SYMS()                                                                      \
	GEN_OFFSET_SYM(soc_esf_t, sio_div_udividend);                                              \
	GEN_OFFSET_SYM(soc_esf_t, sio_div_udivisor);                                               \
	GEN_OFFSET_SYM(soc_esf_t, sio_div_remainder);                                              \
	GEN_OFFSET_SYM(soc_esf_t, sio_div_quotient)
#else
#define GEN_SOC_OFFSET_SYMS()
#endif

#endif /* CONFIG_ARM_SOC_OFFSETS */

#endif /* SOC_ARM_RP2040_SOC_OFFSETS_H_ */
