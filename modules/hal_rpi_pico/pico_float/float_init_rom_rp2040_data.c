#include <pico/bootrom.h>
#include <pico/bootrom/sf_table.h>
#include <zephyr/app_memory/app_memdomain.h>
#include <zephyr/sys/util.h>

#if IS_ENABLED(CONFIG_USERSPACE)
#define FUNCTABLE_ATTR K_APP_DMEM(z_libc_partition)
#else
#define FUNCTABLE_ATTR
#endif

FUNCTABLE_ATTR uint32_t sf_table[SF_TABLE_V2_SIZE / 2];
FUNCTABLE_ATTR void *sf_clz_func;
