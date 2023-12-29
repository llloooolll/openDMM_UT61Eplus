cmake_minimum_required(VERSION 3.8)

# 项目名称
project(eeprom LANGUAGES C ASM)

# 源文件
file(GLOB SOURCE_FILES *.c *.s)
file(GLOB HEADER_FILES *.h)

# 添加源码
add_library(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_link_libraries(${PROJECT_NAME}
    "mcu"
    "si2c"
    "hy2613"
)

# 头文件
target_include_directories(${PROJECT_NAME} PUBLIC
    .
    ${CMAKE_SOURCE_DIR}/source/device/CMSIS/core/include
    ${CMAKE_SOURCE_DIR}/source/device/base
    ${CMAKE_SOURCE_DIR}/source/boot
)
