#ifndef _INCLUDE_PICO_ROM_HELPER_H_
#error Do not include this file directly.
#endif

.macro __pico_init func, priority_string
.section .z_init_EARLY_P_0_SUB_\priority_string\()_
.global __init_\func\()_init
.type __init_\func\()_init,%object
.p2align 2
__init_\func\()_init:
.word \func
.word 0
.size __init_\func\()_init, . - __init_\func\()_init
.endm

#if defined(CONFIG_NEED_LIBC_MEM_PARTITION) && defined(CONFIG_USERSPACE)
#define SECTION_SHARED_DATA(name) \
.section data_smem_z_libc_partition_data
#else
#define SECTION_SHARED_DATA(name) \
.section .data##name
#endif
#define PICO_RUNTIME_INIT_FUNC(func, prio) __pico_init func, prio
