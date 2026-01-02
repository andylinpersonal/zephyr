# SPDX-License-Identifier: Apache-2.0

function(wrap_float_functions)
  cmake_parse_arguments(WRAP_FLOAT "NO_AEABI_ARITHMETIC;NO_AEABI_CMP;NO_AEABI_CONV_32;NO_AEABI_CONV_64;NO_AEABI_CONV_DOUBLE;NO_SQRTF;NO_SCI;NO_SCI_EXTRA" "" "" ${ARGN} )
  if (NOT WRAP_FLOAT_NO_AEABI_ARITHMETIC)
    pico_wrap_function(__aeabi_fadd)
    pico_wrap_function(__aeabi_fdiv)
    pico_wrap_function(__aeabi_fmul)
    pico_wrap_function(__aeabi_frsub)
    pico_wrap_function(__aeabi_fsub)
  endif()
  if (NOT WRAP_FLOAT_NO_AEABI_CMP)
    pico_wrap_function(__aeabi_cfcmpeq)
    pico_wrap_function(__aeabi_cfrcmple)
    pico_wrap_function(__aeabi_cfcmple)
    pico_wrap_function(__aeabi_fcmpeq)
    pico_wrap_function(__aeabi_fcmplt)
    pico_wrap_function(__aeabi_fcmple)
    pico_wrap_function(__aeabi_fcmpge)
    pico_wrap_function(__aeabi_fcmpgt)
    pico_wrap_function(__aeabi_fcmpun)
  endif()
  if (NOT WRAP_FLOAT_NO_AEABI_CONV_32)
    pico_wrap_function(__aeabi_i2f)
    pico_wrap_function(__aeabi_ui2f)
    pico_wrap_function(__aeabi_f2iz)
    pico_wrap_function(__aeabi_f2uiz)
  endif()
  if (NOT WRAP_FLOAT_NO_AEABI_CONV_64)
    pico_wrap_function(__aeabi_l2f)
    pico_wrap_function(__aeabi_ul2f)
    pico_wrap_function(__aeabi_f2lz)
    pico_wrap_function(__aeabi_f2ulz)
  endif()
  if (NOT WRAP_FLOAT_NO_AEABI_CONV_DOUBLE)
    pico_wrap_function(__aeabi_f2d)
  endif()
  # separate as we have a direct DCP version
  if (NOT WRAP_FLOAT_NO_SQRTF)
    pico_wrap_function(sqrtf)
  endif()
  if (NOT WRAP_FLOAT_NO_SCI)
    pico_wrap_function(cosf)
    pico_wrap_function(sinf)
    pico_wrap_function(tanf)
    pico_wrap_function(atan2f)
    pico_wrap_function(expf)
    pico_wrap_function(logf)
    pico_wrap_function(sincosf) # gnu
  endif()
  if (NOT WRAP_FLOAT_NO_SCI_EXTRA)
    pico_wrap_function(ldexpf)
    pico_wrap_function(copysignf)
    pico_wrap_function(truncf)
    pico_wrap_function(floorf)
    pico_wrap_function(ceilf)
    pico_wrap_function(roundf)
    pico_wrap_function(asinf)
    pico_wrap_function(acosf)
    pico_wrap_function(atanf)
    pico_wrap_function(sinhf)
    pico_wrap_function(coshf)
    pico_wrap_function(tanhf)
    pico_wrap_function(asinhf)
    pico_wrap_function(acoshf)
    pico_wrap_function(atanhf)
    pico_wrap_function(exp2f)
    pico_wrap_function(log2f)
    pico_wrap_function(exp10f)
    pico_wrap_function(log10f)
    pico_wrap_function(powf)
    pico_wrap_function(powintf) #gnu
    pico_wrap_function(hypotf)
    pico_wrap_function(cbrtf)
    pico_wrap_function(fmodf)
    pico_wrap_function(dremf)
    pico_wrap_function(remainderf)
    pico_wrap_function(remquof)
    pico_wrap_function(expm1f)
    pico_wrap_function(log1pf)
    pico_wrap_function(fmaf)
  endif()
endfunction()

set(pico_common_dir ${ZEPHYR_HAL_RPI_PICO_MODULE_DIR}/src/common)
set(rp2_common_dir ${ZEPHYR_HAL_RPI_PICO_MODULE_DIR}/src/rp2_common)

zephyr_system_include_directories(${rp2_common_dir}/pico_float/include)

zephyr_compile_definitions(LIB_PICO_FLOAT=1)

if(CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_ROM OR
   CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_DCP OR
   CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_VFP)
  # New library for scoping and ensure pico_sdk depends on it
  set(pico_sdk_target ${ZEPHYR_CURRENT_LIBRARY})
  zephyr_library()
  target_link_libraries(${pico_sdk_target} PRIVATE ${ZEPHYR_CURRENT_LIBRARY})

  target_compile_definitions_ifdef(CONFIG_PICOSDK_SINGLE_FP_PROPAGATE_NANS ${ZEPHYR_CURRENT_LIBRARY}
    PRIVATE "-DPICO_FLOAT_PROPAGATE_NANS=1")
  target_compile_definitions_ifdef(CONFIG_PICOSDK_SINGLE_FP_IN_RAM ${ZEPHYR_CURRENT_LIBRARY}
    PRIVATE "-DPICO_FLOAT_IN_RAM=1")
  target_compile_option_ifdef(CONFIG_PICOSDK_SINGLE_FP_IN_RAM ${ZEPHYR_CURRENT_LIBRARY}
    PRIVATE "-DRAM_SECTION_NAME(x)=.ramfunc.##x")
endif()

if(CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_ROM)
  # Default implementation for RP2040
  zephyr_compile_definitions(LIB_PICO_FLOAT_PICO=1)

  function(set_float_aeabi_rp2040_compiler_args target)
    target_compile_options(${target}
      PRIVATE "-include${pico_module_dir}/pico/pico_rom_helper.h")
    target_compile_definitions_ifdef(CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_ROM_V1 ${target}
      PRIVATE "-DPICO_FLOAT_SUPPORT_ROM_V1=1" "-DPICO_RP2040_B0_SUPPORTED=1")
    target_compile_definitions_ifdef(CONFIG_PICOSDK_SINGLE_FP_PROPAGATE_NANS ${target}
      PRIVATE "-DPICO_FLOAT_PROPAGATE_NANS=1")
    if(CONFIG_PICOSDK_SINGLE_FP_IN_RAM)
      target_compile_options(${target}
        PRIVATE "-DPICO_FLOAT_IN_RAM=1" "-DRAM_SECTION_NAME(x)=.ramfunc.##x")
    endif()
  endfunction()

  # Put init function to init array
  pico_patch_sdk_file(
    PREFIX "float_aeabi_rp2040"
    SOURCE "${rp2_common_dir}/pico_float/float_aeabi_rp2040.S"
    PRE_PATCH_HOOKS  set_float_aeabi_rp2040_compiler_args
  )

  function(set_rp2040_fp_init_compiler_args target)
    target_compile_definitions_ifdef(CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_ROM_V1 ${target}
      PRIVATE "-DPICO_FLOAT_SUPPORT_ROM_V1=1" "-DPICO_RP2040_B0_SUPPORTED=1")
  endfunction()

  # Move the function table to shared data
  macro(set_rp2040_fp_init_objcopy_args)
    set(ret_objcopy_args --weaken-symbol=sf_table)
  endmacro()

  pico_patch_sdk_file(
    PREFIX "rp2040_fp_init"
    SOURCE "${rp2_common_dir}/pico_float/float_init_rom_rp2040.c"
    PRE_PATCH_HOOKS  set_rp2040_fp_init_compiler_args
    GEN_OBJCOPY_ARGS set_rp2040_fp_init_objcopy_args
  )

  target_compile_definitions_ifdef(CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_ROM_V1
    ${ZEPHYR_CURRENT_LIBRARY} PRIVATE "-DPICO_FLOAT_SUPPORT_ROM_V1=1" "-DPICO_RP2040_B0_SUPPORTED=1")

  zephyr_library_sources(
    float_init_rom_rp2040_data.c
    ${rp2_common_dir}/pico_float/float_math.c
    ${rp2_common_dir}/pico_float/float_v1_rom_shim_rp2040.S
  )

  # Wrap all functions with shim
  wrap_float_functions()
elseif(CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_DCP)
  # rp235x/m33 dcp
  zephyr_compile_definitions(LIB_PICO_FLOAT_DCP=1)

  zephyr_library_sources(
    ${rp2_common_dir}/pico_float/float_math.c
    # Following files has ramfunc support.
    ${rp2_common_dir}/pico_float/float_aeabi_dcp.S # TODO
    ${rp2_common_dir}/pico_float/float_common_m33.S
    ${rp2_common_dir}/pico_float/float_sci_m33.S
  )

  # Disable the inlining of single fp arithmetics.
  zephyr_compile_options(-msoft-float)
  # Wrap all functions to avoid the use of VFP.
  wrap_float_functions()
elseif(CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_VFP)
  # rp235x/m33 vfp
  zephyr_compile_definitions(LIB_PICO_FLOAT_VFP=1 LIB_PICO_FLOAT_PICO=1)

  zephyr_library_sources(
    ${rp2_common_dir}/pico_float/float_math.c
    # The following files have IN_RAM support.
    ${rp2_common_dir}/pico_float/float_common_m33.S
    ${rp2_common_dir}/pico_float/float_conv32_vfp.S
    ${rp2_common_dir}/pico_float/float_sci_m33_vfp.S
  )

  wrap_float_functions(
    # for these 3, arguably compiler is probably inlining anyway, but use the cmopiler's
    # version for explicit AEABI calls
    NO_AEABI_ARITHMETIC
    NO_AEABI_CMP
    NO_AEABI_CONV_32
    #NO_AEABI_CONV_64   # we have optimized M33 versions
    NO_AEABI_CONV_DOUBLE
    # we don't have an optimized vfp or m33 sqrtf available
    NO_SQRTF
    #NO_SCI             # we have optimized VFP versions
    #NO_SCI_EXTRA       # todo - are our versions better than what GCC proides?
  )
elseif(CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_HAZARD3)
  zephyr_compile_definitions(LIB_PICO_FLOAT_PICO=1)

  function(set_float_single_hazard3_compiler_args target)
    if(CONFIG_PICOSDK_SINGLE_FP_IN_RAM)
      target_compile_options(${target}
        PRIVATE "-DPICO_FLOAT_IN_RAM=1" "-DRAM_SECTION_NAME(x)=.ramfunc.##x")
    endif()
  endfunction()

  # Replace the default libgcc implementation with pico-sdk's one.
  pico_patch_sdk_file(
    PREFIX "float_single_hazard3"
    SOURCE "${rp2_common_dir}/pico_float/float_single_hazard3.S"
    PRE_PATCH_HOOKS  set_float_single_hazard3_compiler_args
  )
elseif(CONFIG_PICOSDK_USE_SINGLE_FP_IMPL_TRAP)
  zephyr_compile_definitions(LIB_PICO_FLOAT_NONE=1)
  zephyr_library_sources(${rp2_common_dir}/pico_float/float_none.S)
  zephyr_compile_options(-msoft-float)
  wrap_float_functions() # Wrap all functions to trap the calls
else()
  # (Default value when not enabled)
  zephyr_compile_definitions(LIB_PICO_FLOAT_COMPILER=1)
endif()
