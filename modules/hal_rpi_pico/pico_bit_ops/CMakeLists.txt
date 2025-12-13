# SPDX-License-Identifier: Apache-2.0

if(CONFIG_PICOSDK_USE_BITOPS)
  set(pico_common_dir ${ZEPHYR_HAL_RPI_PICO_MODULE_DIR}/src/common)
  set(rp2_common_dir ${ZEPHYR_HAL_RPI_PICO_MODULE_DIR}/src/rp2_common)

  zephyr_system_include_directories(${pico_common_dir}/pico_bit_ops_headers/include)

  function(set_pico_bit_ops_compiler_args target)
    target_compile_option_ifdef(CONFIG_SOC_SERIES_RP2040 ${target}
      PRIVATE "-include${pico_module_dir}/pico/pico_rom_helper.h")
    if(CONFIG_PICOSDK_BITOPS_IN_RAM)
      target_compile_options(${target}
        PRIVATE "-DPICO_BITS_IN_RAM=1" "-DRAM_SECTION_NAME(x)=.ramfunc.##x")
    endif()
  endfunction()

  # Move the function table to shared data
  macro(set_pico_bit_ops_objcopy_args)
    if(CONFIG_SOC_SERIES_RP2040 AND CONFIG_USERSPACE)
      set(ret_objcopy_args --rename-section ".data.aeabi_bits_funcs=${pico_shared_data_section}")
    endif()
  endmacro()

  pico_patch_sdk_file(
    PREFIX "bit_ops_aeabi"
    SOURCE "${rp2_common_dir}/pico_bit_ops/bit_ops_aeabi.S"
    PRE_PATCH_HOOKS  set_pico_bit_ops_compiler_args
    GEN_OBJCOPY_ARGS set_pico_bit_ops_objcopy_args
  )

  if(CONFIG_SOC_SERIES_RP2040)
    # gcc
    pico_wrap_function(__clzsi2)
    pico_wrap_function(__clzsi2)
    pico_wrap_function(__clzdi2)
    pico_wrap_function(__ctzsi2)
    pico_wrap_function(__popcountsi2)
    pico_wrap_function(__popcountdi2)

    # armclang
    pico_wrap_function(__clz)
    pico_wrap_function(__clzl)
    pico_wrap_function(__clzsi2)
    pico_wrap_function(__clzll)
  endif()

  # this is overridden for RP2350 since GCCs impl is bad for both ARM and RISC-V
  pico_wrap_function(__ctzdi2)

endif()

