# SPDX-License-Identifier: Apache-2.0

if(CONFIG_SOC_SERIES_RP2040 AND CONFIG_PICOSDK_USE_ROM_MEMOPS)
  set(rp2_common_dir ${ZEPHYR_HAL_RPI_PICO_MODULE_DIR}/src/rp2_common)

  zephyr_system_include_directories(${rp2_common_dir}/pico_mem_ops/include)

  function(set_pico_mem_ops_compiler_args target)
    target_compile_options(${target}
      PRIVATE "-include${pico_module_dir}/pico/pico_rom_helper.h")
    if(CONFIG_PICOSDK_MEMOPS_IN_RAM)
      target_compile_options(${target}
        PRIVATE "-DPICO_MEM_IN_RAM=1" "-DRAM_SECTION_NAME(x)=.ramfunc.##x")
    endif()
  endfunction()

  # Move the function table to shared data
  macro(set_pico_mem_ops_objcopy_args)
    if(CONFIG_USERSPACE)
      set(ret_objcopy_args --rename-section ".data.aeabi_mem_funcs=${pico_shared_data_section}")
    endif()
  endmacro()

  pico_patch_sdk_file(
    PREFIX "mem_ops_aeabi"
    SOURCE "${rp2_common_dir}/pico_mem_ops/mem_ops_aeabi.S"
    PRE_PATCH_HOOKS  set_pico_mem_ops_compiler_args
    GEN_OBJCOPY_ARGS set_pico_mem_ops_objcopy_args
  )

  #shims for ROM functions for -lgcc functions  (listed below)
  pico_wrap_function(memcpy)
  pico_wrap_function(memset)
  pico_wrap_function(__aeabi_memcpy)
  pico_wrap_function(__aeabi_memset)
  pico_wrap_function(__aeabi_memcpy4)
  pico_wrap_function(__aeabi_memset4)
  pico_wrap_function(__aeabi_memcpy8)
  pico_wrap_function(__aeabi_memset8)
endif()

