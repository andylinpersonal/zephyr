# SPDX-License-Identifier: Apache-2.0

if(CONFIG_SOC_SERIES_RP2040 AND CONFIG_PICOSDK_USE_MUL64)
  set(rp2_common_dir ${ZEPHYR_HAL_RPI_PICO_MODULE_DIR}/src/rp2_common)

  zephyr_system_include_directories(${rp2_common_dir}/pico_int64_ops/include)

  function(set_pico_int64_ops_compiler_args target)
    if(CONFIG_PICOSDK_MUL64_IN_RAM)
      target_compile_options(${target}
        PRIVATE "-DPICO_INT64_OPS_IN_RAM=1" "-DRAM_SECTION_NAME(x)=.ramfunc.##x")
    endif()
  endfunction()

  pico_patch_sdk_file(
    PREFIX "pico_int64_ops"
    SOURCE "${rp2_common_dir}/pico_int64_ops/pico_int64_ops_aeabi.S"
    PRE_PATCH_HOOKS  set_pico_int64_ops_compiler_args
  )

  pico_wrap_function(__aeabi_lmul)
endif()

