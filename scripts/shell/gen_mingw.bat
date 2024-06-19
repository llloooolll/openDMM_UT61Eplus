mkdir out
set ARM_GCC_PATH=%cd%/tool/gcc/bin
cmake -S . -B ./out -G "MinGW Makefiles"
