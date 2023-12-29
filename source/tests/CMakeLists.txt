cmake_minimum_required (VERSION 3.8)

# 子文件夹
add_subdirectory(test_gpio)# 测试GPIO
add_subdirectory(test_sysctrl)# 测试时钟
add_subdirectory(test_si2c)# 测试软件I2C
add_subdirectory(test_hy2613)# 测试HY2316显示
add_subdirectory(test_lcd)# 测试LCD显示
add_subdirectory(test_irda)# 测试红外串口
add_subdirectory(test_buffer)# 测试缓冲区
add_subdirectory(test_qpn)# 测试QP-nano
add_subdirectory(test_eeprom)# 测试EEPROM
