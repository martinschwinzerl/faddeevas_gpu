cmake_minimum_required( VERSION 3.7 FATAL_ERROR )
project( faddeevas_opencl LANGUAGES C CXX )

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE "Release" CACHE STRING
       "Build type [Debug ReleaseRelWithDebInfo MinSizeRel]" FORCE )
endif()

add_library( faddeevas_opencl_common_cxx OBJECT
    common/definitions.h common/benchmark_item.h common/benchmark_item.cpp
    common/helper_functions.h )

target_include_directories( faddeevas_opencl_common_cxx
    PRIVATE ${CMAKE_SOURCE_DIR} )

set_target_properties( faddeevas_opencl_common_cxx PROPERTIES
    CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON )


add_library( faddeevas_opencl_common_c OBJECT
    common/definitions.h common/helper_functions.h common/helper_functions.c )

target_include_directories( faddeevas_opencl_common_c
    PRIVATE ${CMAKE_SOURCE_DIR} )

find_package( OpenCL REQUIRED )


# set( CERRF_OPENCL_RUNTIME_COMPILE_OPTIONS "-save-temps" )
# configure_file( run_cerrf.h.template ${CMAKE_SOURCE_DIR}/run_cerrf.h @ONLY )

# ==============================================================================

add_executable( run_wofz_opencl run_opencl.cpp
    $<TARGET_OBJECTS:faddeevas_opencl_common_cxx>
    $<TARGET_OBJECTS:faddeevas_opencl_common_c> )

target_include_directories( run_wofz_opencl
    PRIVATE ${CMAKE_SOURCE_DIR} )

set_target_properties( run_wofz_opencl
    PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON )

target_link_libraries( run_wofz_opencl OpenCL::OpenCL )
target_compile_definitions( run_wofz_opencl PUBLIC -DCL_HPP_ENABLE_EXCEPTIONS )
target_compile_definitions( run_wofz_opencl PUBLIC -DCL_HPP_TARGET_OPENCL_VERSION=300 )


add_executable( run_wofz_opencl_abrarov_quine_2011 run_opencl_abrarov_quine.cpp
    abrarov_quine_2011/definitions.h
    abrarov_quine_2011/coefficients.h
    abrarov_quine_2011/ErrorFunctions.h
    abrarov_quine_2011/ErrorFunctions.c
    $<TARGET_OBJECTS:faddeevas_opencl_common_cxx>
    $<TARGET_OBJECTS:faddeevas_opencl_common_c> )

target_include_directories( run_wofz_opencl_abrarov_quine_2011
    PRIVATE ${CMAKE_SOURCE_DIR} )

set_target_properties( run_wofz_opencl_abrarov_quine_2011
    PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON )

target_link_libraries( run_wofz_opencl_abrarov_quine_2011 OpenCL::OpenCL )
target_compile_definitions( run_wofz_opencl_abrarov_quine_2011 PUBLIC -DCL_HPP_ENABLE_EXCEPTIONS )
target_compile_definitions( run_wofz_opencl_abrarov_quine_2011 PUBLIC -DCL_HPP_TARGET_OPENCL_VERSION=300 )

# add_executable( run_cerrf run_cerrf.cpp )
# set_target_properties( run_cerrf PROPERTIES
#     POSITION_INDEPENDENT_CODE ON CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON )
