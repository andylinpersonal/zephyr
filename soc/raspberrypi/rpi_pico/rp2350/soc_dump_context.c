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
#if defined(CONFIG_PICOSDK_USE_DCP_COPROC)
	EXCEPTION_DUMP("dcp_xm: 0x%08x%08x", soc_context->dcp_xmh, soc_context->dcp_xml);
	EXCEPTION_DUMP("dcp_ym: 0x%08x%08x", soc_context->dcp_ymh, soc_context->dcp_yml);
	EXCEPTION_DUMP("dcp_exp_flag_status: h: 0x%08x l: 0x%08x",
		       soc_context->dcp_exp_flag_statush, soc_context->dcp_exp_flag_statusl);
#endif
}
