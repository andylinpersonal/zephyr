# SPDX-License-Identifier: Apache-2.0

function(wrap_double_functions)
  pico_wrap_function(__aeabi_dadd)
  pico_wrap_function(__aeabi_ddiv)
  pico_wrap_function(__aeabi_dmul)
  pico_wrap_function(__aeabi_drsub)
  pico_wrap_function(__aeabi_dsub)
  pico_wrap_function(__aeabi_cdcmpeq)
  pico_wrap_function(__aeabi_cdrcmple)
  pico_wrap_function(__aeabi_cdcmple)
  pico_wrap_function(__aeabi_dcmpeq)
  pico_wrap_function(__aeabi_dcmplt)
  pico_wrap_function(__aeabi_dcmple)
  pico_wrap_function(__aeabi_dcmpge)
  pico_wrap_function(__aeabi_dcmpgt)
  pico_wrap_function(__aeabi_dcmpun)
  pico_wrap_function(__aeabi_i2d)
  pico_wrap_function(__aeabi_l2d)
  pico_wrap_function(__aeabi_ui2d)
  pico_wrap_function(__aeabi_ul2d)
  pico_wrap_function(__aeabi_d2iz)
  pico_wrap_function(__aeabi_d2lz)
  pico_wrap_function(__aeabi_d2uiz)
  pico_wrap_function(__aeabi_d2ulz)
  pico_wrap_function(__aeabi_d2f)
  pico_wrap_function(sqrt)
  pico_wrap_function(cos)
  pico_wrap_function(sin)
  pico_wrap_function(tan)
  pico_wrap_function(atan2)
  pico_wrap_function(exp)
  pico_wrap_function(log)

  pico_wrap_function(ldexp)
  pico_wrap_function(copysign)
  pico_wrap_function(trunc)
  pico_wrap_function(floor)
  pico_wrap_function(ceil)
  pico_wrap_function(round)
  pico_wrap_function(sincos) # gnu
  pico_wrap_function(asin)
  pico_wrap_function(acos)
  pico_wrap_function(atan)
  pico_wrap_function(sinh)
  pico_wrap_function(cosh)
  pico_wrap_function(tanh)
  pico_wrap_function(asinh)
  pico_wrap_function(acosh)
  pico_wrap_function(atanh)
  pico_wrap_function(exp2)
  pico_wrap_function(log2)
  pico_wrap_function(exp10)
  pico_wrap_function(log10)
  pico_wrap_function(pow)
  pico_wrap_function(powint) #gnu
  pico_wrap_function(hypot)
  pico_wrap_function(cbrt)
  pico_wrap_function(fmod)
  pico_wrap_function(drem)
  pico_wrap_function(remainder)
  pico_wrap_function(remquo)
  pico_wrap_function(expm1)
  pico_wrap_function(log1p)
  pico_wrap_function(fma)
endfunction()

set(pico_common_dir ${ZEPHYR_HAL_RPI_PICO_MODULE_DIR}/src/common)
set(rp2_common_dir ${ZEPHYR_HAL_RPI_PICO_MODULE_DIR}/src/rp2_common)

zephyr_system_include_directories(${rp2_common_dir}/pico_double/include)

zephyr_compile_definitions(LIB_PICO_DOUBLE=1)

if(CONFIG_PICOSDK_USE_DOUBLE_FP_IMPL_ROM OR
   CONFIG_PICOSDK_USE_DOUBLE_FP_IMPL_DCP)
  set(pico_sdk_target ${ZEPHYR_CURRENT_LIBRARY})
  zephyr_library()
  target_link_libraries(${pico_sdk_target} PRIVATE ${ZEPHYR_CURRENT_LIBRARY})

  target_compile_definitions_ifdef(CONFIG_PICOSDK_DOUBLE_FP_PROPAGATE_NANS ${ZEPHYR_CURRENT_LIBRARY}
    PRIVATE "-DPICO_DOUBLE_PROPAGATE_NANS=1")
  target_compile_definitions_ifdef(CONFIG_PICOSDK_DOUBLE_FP_IN_RAM ${ZEPHYR_CURRENT_LIBRARY}
    PRIVATE "-DPICO_DOUBLE_IN_RAM=1")
  target_compile_option_ifdef(CONFIG_PICOSDK_DOUBLE_FP_IN_RAM ${ZEPHYR_CURRENT_LIBRARY}
    PRIVATE "-DRAM_SECTION_NAME(x)=.ramfunc.##x")
endif()

if(CONFIG_PICOSDK_USE_DOUBLE_FP_IMPL_ROM)
  zephyr_compile_definitions(LIB_PICO_DOUBLE_PICO=1)

  function(set_double_aeabi_rp2040_compiler_args target)
    target_compile_options(${target}
      PRIVATE "-include${pico_module_dir}/pico/pico_rom_helper.h")
    target_compile_definitions_ifdef(CONFIG_PICOSDK_USE_DOUBLE_FP_IMPL_ROM_V1 ${target}
      PRIVATE "-DPICO_FLOAT_SUPPORT_ROM_V1=1" "-DPICO_RP2040_B0_SUPPORTED=1")
    target_compile_definitions_ifdef(CONFIG_PICOSDK_DOUBLE_FP_PROPAGATE_NANS ${target}
      PRIVATE "-DPICO_DOUBLE_PROPAGATE_NANS=1")
    if(CONFIG_PICOSDK_DOUBLE_FP_IN_RAM)
      target_compile_options(${target}
        PRIVATE "-DPICO_DOUBLE_IN_RAM=1" "-DRAM_SECTION_NAME(x)=.ramfunc.##x")
    endif()
  endfunction()

  # Put init function to init array
  pico_patch_sdk_file(
    PREFIX "double_aeabi_rp2040"
    SOURCE "${rp2_common_dir}/pico_double/double_aeabi_rp2040.S"
    PRE_PATCH_HOOKS  set_double_aeabi_rp2040_compiler_args
  )

  function(set_rp2040_double_fp_init_compiler_args target source)
    target_compile_definitions_ifdef(CONFIG_PICOSDK_USE_DOUBLE_FP_IMPL_ROM_V1 ${target}
      PRIVATE "-DPICO_DOUBLE_SUPPORT_ROM_V1=1" "-DPICO_RP2040_B0_SUPPORTED=1")
    set_source_files_properties(
      "${source}"
      PROPERTIES
        COMPILE_FLAGS $<TARGET_PROPERTY:compiler,warning_no_pointer_arithmetic>
    )
  endfunction()

  # Move the function table to shared data
  macro(set_rp2040_double_fp_init_objcopy_args)
    set(ret_objcopy_args --weaken-symbol=sd_table)
  endmacro()

  pico_patch_sdk_file(
    PREFIX "rp2040_double_fp_init"
    SOURCE "${rp2_common_dir}/pico_double/double_init_rom_rp2040.c"
    PRE_PATCH_HOOKS  set_rp2040_double_fp_init_compiler_args
    GEN_OBJCOPY_ARGS set_rp2040_double_fp_init_objcopy_args
  )

  target_compile_definitions_ifdef(CONFIG_PICOSDK_USE_DOUBLE_FP_IMPL_ROM_V1
    ${ZEPHYR_CURRENT_LIBRARY} PRIVATE "-DPICO_DOUBLE_SUPPORT_ROM_V1=1" "-DPICO_RP2040_B0_SUPPORTED=1")

  zephyr_library_sources(
    double_init_rom_rp2040_data.c
    ${rp2_common_dir}/pico_double/double_math.c
    ${rp2_common_dir}/pico_double/double_v1_rom_shim_rp2040.S
  )
  wrap_double_functions()
elseif(CONFIG_PICOSDK_USE_DOUBLE_FP_IMPL_DCP)
  # rp235x/m33 dcp
  zephyr_compile_definitions(LIB_PICO_DOUBLE_PICO=1)

  zephyr_library_sources(
    ${rp2_common_dir}/pico_double/double_math.c
    # Following files has ramfunc support.
    ${rp2_common_dir}/pico_double/double_aeabi_dcp.S
    ${rp2_common_dir}/pico_double/double_fma_dcp.S
    ${rp2_common_dir}/pico_double/double_sci_m33.S
    ${rp2_common_dir}/pico_double/double_conv_m33.S
  )

  wrap_double_functions()
elseif(CONFIG_PICOSDK_USE_DOUBLE_FP_IMPL_TRAP)
  zephyr_compile_definitions(LIB_PICO_DOUBLE_NONE=1)
  zephyr_library_sources(${rp2_common_dir}/pico_double/double_none.S)
  wrap_double_functions()
else()
  zephyr_compile_definitions(LIB_PICO_DOUBLE_COMPILER=1)
endif()

