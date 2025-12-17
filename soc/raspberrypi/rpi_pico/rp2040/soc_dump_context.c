/*
 * Copyright (c) 2025 Andy Lin
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/arch/exception.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(os, CONFIG_KERNEL_LOG_LEVEL);

void __soc_dump_context(soc_esf_t *soc_context)
{
#if IS_ENABLED(CONFIG_RP2_USE_SIO_DIVIDER)
	EXCEPTION_DUMP("SIO_DIV_UDIVIDEND: 0x%08x", soc_context->sio_div_udividend);
	EXCEPTION_DUMP("SIO_DIV_UDIVISOR:  0x%08x", soc_context->sio_div_udivisor);
	EXCEPTION_DUMP("SIO_DIV_REMAINDER: 0x%08x", soc_context->sio_div_remainder);
	EXCEPTION_DUMP("SIO_DIV_QUOTIENT:  0x%08x", soc_context->sio_div_quotient);
#endif
}
