import tkinter
from tkinter import filedialog
import os

root = tkinter.Tk()

# 寻找原始txt文件
# 格式：01 02 03 0F FF ...
src_file_name = filedialog.askopenfilename()
print(f"from file: {src_file_name}")

# 创建bin转换后的文件
name_offset = 0
while True:
    dest_file_name = src_file_name + '_' + str(name_offset) + ".bin"
    if not os.path.exists(dest_file_name):
        print(f"to file: {dest_file_name}")
        break
    else:
        name_offset += 1

# 打开文件
file_src = open(src_file_name, "r")
file_dest = open(dest_file_name, "ab")

while True:
    data_byte = file_src.read(3)
    if not data_byte:
        break
    data_int = int(data_byte, 16)
    print(f"{data_byte} -> {data_int}")

    file_dest.write(bytes([data_int]))

file_src.close()
file_dest.close()