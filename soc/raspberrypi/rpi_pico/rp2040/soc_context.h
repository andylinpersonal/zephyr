/*
 * Copyright (c) 2025 Andy Lin
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SOC_ARM_RP2040_SOC_CONTEXT_H_
#define SOC_ARM_RP2040_SOC_CONTEXT_H_

#ifdef CONFIG_ARM_SOC_CONTEXT_SAVE

#if defined(CONFIG_RP2_USE_SIO_DIVIDER)
#define SIO_DIV_CONTEXT_MEMBERS                                                                    \
	uint32_t sio_div_udividend;                                                                \
	uint32_t sio_div_udivisor;                                                                 \
	uint32_t sio_div_remainder;                                                                \
	uint32_t sio_div_quotient

#define SIO_DIV_CONTEXT_INIT 0, 0, 0, 0

#define SOC_ESF_MEMBERS SIO_DIV_CONTEXT_MEMBERS
#define SOC_ESF_INIT    SIO_DIV_CONTEXT_INIT
#else
#define SOC_ESF_MEMBERS uint32_t dummy
#define SOC_ESF_INIT    0
#endif

#endif /* CONFIG_ARM_SOC_CONTEXT_SAVE */

#endif /* SOC_ARM_RP2040_SOC_CONTEXT_H_ */
