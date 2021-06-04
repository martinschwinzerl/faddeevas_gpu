# -------------------------------------------------------------------------------
# Enable HIP support

# Set environment variable CUDA_BIN_PATH to the directory containing nvcc or
# HIP_ROOT_DIR variable to hint the CUDA install location

if( NOT DEFINED HIP_PATH )
    if( NOT DEFINED ENV{HIP_PATH} )
        set( HIP_PATH "/opt/rocm/hip" CACHE PATH
             "Path to which HIP has been installed" FORCE )
    else()
        set( HIP_PATH $ENV{HIP_PATH} CACHE PATH
             "Path to which HIP has been installed" FORCE )
    endif()
endif()

set(CMAKE_MODULE_PATH "${HIP_PATH}/cmake" ${CMAKE_MODULE_PATH})

find_package( HIP REQUIRED )

if(HIP_FOUND)
    set(HIP_ENABLED ON)
    message(STATUS "HIP Found (${hip_HIPCC_EXECUTABLE})")
    #set(HIP_CMAKE_CXX_FLAGS "-fcuda-flush-denormals-to-zero -Wno-invalid-command-line-argument -Wno-unused-command-line-argument -Wno-invalid-constexpr -Wno-ignored-optimization-argument -Wno-unused-private-field")

    if( DEFINED HIP_AMDGPUTARGET )
        if("${HIP_PLATFORM}" STREQUAL hcc)
            set( HIP_CMAKE_CXX_FLAGS
                 "${HIP_CMAKE_CXX_FLAGS} --amdgpu-target=${HIP_AMDGPUTARGET}")
        elseif("${HIP_PLATFORM}" STREQUAL nvcc)
            set( HIP_CMAKE_CXX_FLAGS
                "${HIP_CMAKE_CXX_FLAGS} -arch=${HIP_AMDGPUTARGET}" )
        endif()
    else()
        if( "${HIP_PLATFORM}" STREQUAL hcc )
            set( HIP_AMDGPUTARGET "gfx906" )
            set( HIP_CMAKE_CXX_FLAGS
                "${HIP_CMAKE_CXX_FLAGS} --amdgpu-target=gfx906" )
        elseif("${HIP_PLATFORM}" STREQUAL nvcc)
            set( HIP_AMDGPUTARGET "compute_70" )
            set( HIP_CMAKE_CXX_FLAGS
                "${HIP_CMAKE_CXX_FLAGS} -arch=compute_70" )
        endif()
    endif()
endif()

set( CMAKE_CXX_STANDARD_REQUIRED ON )

if( "${HIP_PLATFORM}" STREQUAL hcc )
    set( CMAKE_POSITION_INDEPENDENT_CODE ON )
    set( CMAKE_CXX_STANDARD 11 )
    set( CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} ${HIP_CMAKE_CXX_FLAGS} -fno-gpu-rdc -fno-fast-math" )
    message( STATUS "Building demo with HIP support ${HIP_AMDGPUTARGET}" )
elseif( "${HIP_PLATFORM}" STREQUAL nvcc )
    set( HIP_NVCC_FLAG "-x cu -arch=${HIP_AMDGPUTARGET}" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${HIP_NVCC_FLAG}" )
    message( STATUS "Building demo CUDA with HIP support ${HIP_AMDGPUTARGET}" )
endif()

include_directories( "${CMAKE_SOURCE_DIR}/hip" )

HIP_ADD_EXECUTABLE( cerrf_base_hip run_cerrf_base.cpp )
HIP_ADD_EXECUTABLE( cerrf_optimised_hip run_cerrf_optimised.cpp )
