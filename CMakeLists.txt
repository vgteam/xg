cmake_minimum_required(VERSION 3.22...4.2)

project(xg
    LANGUAGES CXX
)

include(FetchContent)
include(ExternalProject)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)

# get system libraries
find_package(OpenMP REQUIRED)

# FetchContent deps
FetchContent_Declare(
    libhandlegraph
    GIT_REPOSITORY https://github.com/vgteam/libhandlegraph
    GIT_TAG        ae42926da7743a7108db0474bfcf1a1cdac34abb
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(sdsl-lite
    GIT_REPOSITORY https://github.com/vgteam/sdsl-lite.git
    GIT_TAG        vg-cmake-hublabel
    EXCLUDE_FROM_ALL
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Do not build a shared library for libdivsufsort")
set(BUILD_EXAMPLES OFF CACHE BOOL "Do not build libdivsufsort example")
set(BUILD_DIVSUFSORT64 ON CACHE BOOL "Build libdivsufsort in 64-bits mode")
FetchContent_Declare(libdivsufsort
    GIT_REPOSITORY https://github.com/vgteam/libdivsufsort.git
    GIT_TAG        5ad79590572480e6d480703db99f070739f06961
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(mmmulti
    GIT_REPOSITORY https://github.com/vgteam/mmmulti.git
    GIT_TAG        b7b399aba05015417501fcf6b2c697fe0559b853
    EXCLUDE_FROM_ALL
)
set(IPS4O_OPTIMIZE_FOR_NATIVE OFF)
FetchContent_Declare(ips4o
    GIT_REPOSITORY https://github.com/electricEpilith/ips4o.git
    GIT_TAG        2af2fd6899e04fe9158d65ee6247be92df45be7d
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(atomic_queue
    GIT_REPOSITORY https://github.com/max0x7ba/atomic_queue.git
    GIT_TAG        f68d9800936d15484c05d583c74060fb3811d8f8
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(tayweeargs
    GIT_REPOSITORY https://github.com/Taywee/args.git
    GIT_TAG        866c4d3c2e891a0887009b5bef5e09fcbcb13b3c
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(gfakluge
    GIT_REPOSITORY https://github.com/vgteam/gfakluge.git
    GIT_TAG        041f90955774c49f99a7bde3c7173a467c3b5f1c
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(
    libhandlegraph
    sdsl-lite
    libdivsufsort
    mmmulti
    ips4o
    atomic_queue
    tayweeargs
    gfakluge
)

# mmmulti pulls in ips4o (needs _OPENMP for parallel::sort) and paryfor
# (uses `Base::template foo(...)` syntax that newer clang diagnoses).
target_link_libraries(mmmulti_lib PUBLIC OpenMP::OpenMP_CXX)
target_compile_options(mmmulti_lib PRIVATE -Wno-missing-template-arg-list-after-template-kw)

add_library(libxg_static STATIC
    src/xg.cpp
)
target_include_directories(libxg_static PUBLIC src)
target_link_libraries(libxg_static PRIVATE
    handlegraph_static sdsl
    divsufsort divsufsort64
    mmmulti_lib ips4o
    mio::mio-headers
    atomic_queue gfakluge
)