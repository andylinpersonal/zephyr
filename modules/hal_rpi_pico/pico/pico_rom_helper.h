#ifndef _INCLUDE_PICO_ROM_HELPER_H_
#define _INCLUDE_PICO_ROM_HELPER_H_ 1

#ifndef __ASSEMBLER__
#include <zephyr/init.h>

#define PICO_RUNTIME_INIT_FUNC(func, priority_string)                                              \
	static const Z_DECL_ALIGN(struct init_entry) __attribute__((                               \
		section(".z_init_EARLY_P_0_SUB_" priority_string "_"))) __used __noasan            \
	Z_INIT_ENTRY_NAME(func) = {.init_fn = (func), .dev = NULL}
#else
#include "pico_rom_helper.S"
#endif
#endif /* !_INCLUDE_PICO_ROM_HELPER_H_ */
