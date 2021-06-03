cmake_minimum_required( VERSION 3.7 FATAL_ERROR )
project( faddeevas_opencl LANGUAGES C CXX )

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE "Release" CACHE STRING
       "Build type [Debug ReleaseRelWithDebInfo MinSizeRel]" FORCE )
endif()

find_package( OpenCL REQUIRED )

set( CERRF_OPENCL_RUNTIME_COMPILE_OPTIONS "-save-temps" )

configure_file( run_cerrf.h.template ${CMAKE_SOURCE_DIR}/run_cerrf.h @ONLY )

# ==============================================================================

add_executable( run_cerrf run_cerrf.cpp )
set_target_properties( run_cerrf PROPERTIES
    POSITION_INDEPENDENT_CODE ON CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON )
target_compile_definitions( run_cerrf PUBLIC -DCL_HPP_ENABLE_EXCEPTIONS )
target_link_libraries( run_cerrf OpenCL::OpenCL )


