cmake_minimum_required(VERSION 3.8)

# 项目名称
project(qpn LANGUAGES C ASM)

# 源文件
set(SOURCE_FILES
    "./src/qfn/qfn.c"
    "./src/qfn/qepn.c"
    "./src/qvn/qvn.c"
    "./ports/arm-cm/qv/gnu/qvn_port.c"
)

# 添加源码
add_library(${PROJECT_NAME}
    ${SOURCE_FILES}
)

# 头文件
target_include_directories(${PROJECT_NAME} PUBLIC
    .
    ./include
    ./ports/arm-cm/qv/gnu
    ${CMAKE_SOURCE_DIR}/source/device/CMSIS/core/include
    ${CMAKE_SOURCE_DIR}/source/device/base
    ${CMAKE_SOURCE_DIR}/source/boot
)
