cmake_minimum_required(VERSION 3.8)

# 源文件
file(GLOB SOURCE_FILES *.c *.s)
file(GLOB HEADER_FILES *.h)

# 项目名称
project(hc32l130_lib LANGUAGES C ASM)

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
