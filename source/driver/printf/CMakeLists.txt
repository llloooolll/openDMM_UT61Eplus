cmake_minimum_required(VERSION 3.8)

# 项目名称
project(printf LANGUAGES C ASM)

# 源文件
file(GLOB SOURCE_FILES *.c *.s)

# 添加源码
add_library(${PROJECT_NAME}
    ${SOURCE_FILES}
)

# 头文件
target_include_directories(${PROJECT_NAME} PUBLIC
    .
)

# 库
target_link_libraries(${PROJECT_NAME}
    "mcu"
)

add_definitions(
    -DPRINTF_INCLUDE_CONFIG_H
)
