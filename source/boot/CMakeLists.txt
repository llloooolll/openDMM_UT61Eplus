﻿cmake_minimum_required(VERSION 3.8)

# 项目名称
project(boot LANGUAGES C ASM)

# 源文件
file(GLOB SOURCE_FILES *.c *.s)

# 添加源码
add_library(${PROJECT_NAME}
    ${SOURCE_FILES}
)

# 头文件
target_include_directories(${PROJECT_NAME} PUBLIC
    .
    ${CMAKE_SOURCE_DIR}/source/device/CMSIS/core/include
)
