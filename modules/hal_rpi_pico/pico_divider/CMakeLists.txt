# SPDX-License-Identifier: Apache-2.0

if(CONFIG_PICOSDK_USE_HW_DIVIDER AND CONFIG_SOC_SERIES_RP2040)
  set(pico_common_dir ${ZEPHYR_HAL_RPI_PICO_MODULE_DIR}/src/common)
  set(rp2_common_dir ${ZEPHYR_HAL_RPI_PICO_MODULE_DIR}/src/rp2_common)

  function(set_pico_divider_compiler_args target)
    if(CONFIG_PICOSDK_DIVIDER_IN_RAM)
      target_compile_options(${target}
        PRIVATE "-DPICO_DIVIDER_IN_RAM=1" "-DRAM_SECTION_NAME(x)=.ramfunc.##x")
    endif()
  endfunction()

  macro(set_pico_divider_objcopy_args)
    set(ret_objcopy_args
      # Wrap around the buggy unsafe variant in pico-sdk. (Issue #2773)
      --redefine-sym divmod_u32u32_unsafe=divmod_u32u32_unsafe_bad
      --redefine-sym divmod_s32s32_unsafe=divmod_s32s32_unsafe_bad
      --strip-symbol=__wrap___aeabi_idiv
      --strip-symbol=__wrap___aeabi_idivmod
      --strip-symbol=__wrap___aeabi_uidiv
      --strip-symbol=__wrap___aeabi_uidivmod
      # Cannot easily `move' symbols in CMake.
      --strip-symbol=__wrap___aeabi_ldivmod
      --strip-symbol=__wrap___aeabi_uldivmod)
  endmacro()

  pico_patch_sdk_file(
    PREFIX "pico_divider"
    SOURCE "${rp2_common_dir}/pico_divider/divider_hardware.S"
    PRE_PATCH_HOOKS  set_pico_divider_compiler_args
    GEN_OBJCOPY_ARGS set_pico_divider_objcopy_args
  )

  set(pico_sdk_target ${ZEPHYR_CURRENT_LIBRARY})
  zephyr_library()
  target_link_libraries(${pico_sdk_target} PRIVATE ${ZEPHYR_CURRENT_LIBRARY})

  zephyr_library_sources(
    "divider_hardware_patch.S"
  )

  # Public headers and options
  zephyr_include_directories(${rp2_common_dir}/hardware_divider/include)
  zephyr_include_directories(${pico_common_dir}/pico_divider_headers/include)

  pico_wrap_function(__aeabi_idiv)
  pico_wrap_function(__aeabi_idivmod)
  pico_wrap_function(__aeabi_ldivmod)
  pico_wrap_function(__aeabi_uidiv)
  pico_wrap_function(__aeabi_uidivmod)
  pico_wrap_function(__aeabi_uldivmod)
endif()
