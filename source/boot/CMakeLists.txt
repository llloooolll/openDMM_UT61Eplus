cmake_minimum_required(VERSION 3.8)

# 项目名称
project(boot_main LANGUAGES C ASM)

# 源文件
file(GLOB SOURCE_FILES *.c *.s)
file(GLOB HEADER_FILES *.h)

# 添加源码
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

# 头文件
target_include_directories(${PROJECT_NAME} PRIVATE
    .
    ${CMAKE_SOURCE_DIR}/source/device/CMSIS/core/include
)

include(${CMAKE_SOURCE_DIR}/scripts/cmake/utilities.cmake)
set_target_linker(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/scripts/ld/hc32l130.ld)
post_build(${PROJECT_NAME})

# 项目名称
project(boot LANGUAGES C ASM)

# 添加源码
add_library(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

# 头文件
target_include_directories(${PROJECT_NAME} PUBLIC
    .
    ${CMAKE_SOURCE_DIR}/source/device/CMSIS/core/include
)
