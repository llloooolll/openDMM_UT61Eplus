cmake_minimum_required(VERSION 3.8)

set(CMAKE_C_STANDARD 99)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/scripts/cmake/arm-gcc-cortex-m0plus.cmake")
include(${CMAKE_SOURCE_DIR}/scripts/cmake/utilities.cmake)

project(openDMM_UT61Eplus C ASM)

# 子文件夹
add_subdirectory(source)
