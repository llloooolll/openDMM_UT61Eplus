mkdir build
set ARM_GCC_PATH=%cd%/tool/gcc/bin
cmake -S . -B ./build -G "Ninja"
