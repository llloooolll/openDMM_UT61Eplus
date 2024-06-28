# openDMM_UT61Eplus

## 编译步骤
- 下载arm-none-eabi-gcc，放入tool/gcc
- 安装Cmake
- 运行./scripts/shell/gen_mingw.bat
- 运行./scripts/shell/build.bat all

## 下载
- 更换同型号MCU，以免无法恢复到原来的功能
- 连接SWD线
- 安装pyocd，安装HC32L13x的pack包
- 运行./scripts/shell/load out/source/app/app.hex

|芯片   |型号|
|---    |--- |
|MCU    |HC32L130J8TA|
|LCD    |HY2613B|
|DMM    |ES232 + ES5|
|EEPROM |AT24C02|

|档位   |测量 | 校准|
|---    |--- |--- |
|交流电压       |YES  |NO  |
|直流电压       |YES  |YES  |
|直流毫伏电压    |YES  |NO  |
|欧姆           |YES  |NO  |
|通断           |YES  |NO  |
|二极管         |YES  |YES  |
|电容           |YES  |NO  |
|频率           |NO  |NO  |
|占空比         |NO  |NO  |
|三极管         |NO  |NO  |
|直流微安       |YES  |NO  |
|交流微安       |YES  |NO  |
|直流毫安       |YES  |NO  |
|交流毫安       |YES  |NO  |
|直流安培       |YES  |NO  |
|交流安培       |YES  |NO  |
|非接触测量     |YES  |NO  |
