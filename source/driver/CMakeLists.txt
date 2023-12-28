cmake_minimum_required (VERSION 3.8)

# 子文件夹
add_subdirectory(si2c)# 软件I2C
add_subdirectory(lcd)# LCD驱动
add_subdirectory(irda)# 红外串口驱动
add_subdirectory(qpn)# QP nano
add_subdirectory(buffer)# 软件缓冲区实现
