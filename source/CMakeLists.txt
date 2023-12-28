cmake_minimum_required (VERSION 3.8)

# 子文件夹
add_subdirectory(boot)# 单片机启动代码
add_subdirectory(device)# 单片机硬件相关代码
add_subdirectory(tests)# 测试例程
add_subdirectory(driver)# 驱动程序
add_subdirectory(app)# 主程序
