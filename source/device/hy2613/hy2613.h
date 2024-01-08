#ifndef __HY2613_H__
#define __HY2613_H__

#include "hc32l13x.h"

/* LCD地址 */
#define HY2613_I2C_ADDR 0x7C

/* LCD命令/数据 */
#define HY2613_NEXT_CMD 0x80   // 下一字节是命令
#define HY2613_NEXT_DATA 0x00  // 下一字节是数据

/* LCD指令 */
#define HY2613_CMD_MODESET 0x40
#define HY2613_CMD_ADSET 0x00
#define HY2613_CMD_DISCTL 0x20
#define HY2613_CMD_ICSET 0x68
#define HY2613_CMD_BLKCTL 0x70
#define HY2613_CMD_APCTL 0x7C
#define HY2613_CMD_EXCTL 0x60

/*HY2613_CMD_MODESET*/
// 显示开关
#define HY2613_CMD_MODESET_DIS_OFF 0x00  // 开显示
#define HY2613_CMD_MODESET_DIS_ON 0x08   // 关显示
// 驱动波形设置
#define HY2613_CMD_MODESET_BIAS_1D3 0x00  // 1/3 BIAS
#define HY2613_CMD_MODESET_BIAS_1D2 0x04  // 1/2 BIAS

/*HY2613_CMD_ADSET*/
// 像素数据偏移地址
#define HY2613_CMD_ADSET_ADDR 0x00

/*HY2613_CMD_DISCTL*/
// 工作电流
#define HY2613_CMD_DISCTL_PWR_FR_MODE_0 0x00  // 最大，默认
#define HY2613_CMD_DISCTL_PWR_FR_MODE_1 0x08  //
#define HY2613_CMD_DISCTL_PWR_FR_MODE_2 0x10  //
#define HY2613_CMD_DISCTL_PWR_FR_MODE_3 0x18  //
// 驱动波形
#define HY2613_CMD_DISCTL_WAVE_LINE 0x00
#define HY2613_CMD_DISCTL_WAVE_FRAME 0x04
// 省电模式
#define HY2613_CMD_DISCTL_PWR_SR_MODE_1 0x00  // 电流最小
#define HY2613_CMD_DISCTL_PWR_SR_MODE_2 0x01  //
#define HY2613_CMD_DISCTL_PWR_SR_MODE_N 0x02  //
#define HY2613_CMD_DISCTL_PWR_SR_MODE_H 0x03  //

/*HY2613_CMD_ICSET*/
// 软复位设置
#define HY2613_CMD_ICSET_NRST 0x00  // 不复位
#define HY2613_CMD_ICSET_RST 0x02   // 复位
// 时钟源设置
#define HY2613_CMD_ICSET_OSC_IN 0x00  // 内部
#define HY2613_CMD_ICSET_OSC_EX 0x01  // 外部
// 地址高位设置
#define HY2613_CMD_ICSET_ADDR_MSB_L 0x00  // 00-1F
#define HY2613_CMD_ICSET_ADDR_MSB_H 0x04  // 20-23

/*HY2613_CMD_BLKCTL*/
// 屏幕闪烁设置
#define HY2613_CMD_BLKCTL_BLK_OFF 0x00  // 不闪
#define HY2613_CMD_BLKCTL_BLK_0D5 0x01  // 0.5Hz
#define HY2613_CMD_BLKCTL_BLK_1 0x02    // 1Hz
#define HY2613_CMD_BLKCTL_BLK_2 0x03    // 2Hz

/*HY2613_CMD_APCTL*/
// 像素显示设置
#define HY2613_CMD_APCTL_PIXEL_N 0x00     // 正常，根据RAM数据
#define HY2613_CMD_APCTL_PIXEL_OFF 0x01   // 全不显示
#define HY2613_CMD_APCTL_PIXEL_ON 0x02    // 全显示
#define HY2613_CMD_APCTL_PIXEL_OFF1 0x03  // 同0x01

/*HY2613_CMD_EXCTL*/
// 背光驱动设置
#define HY2613_CMD_EXCTL_BL_OFF 0x00  // 关
#define HY2613_CMD_EXCTL_BL_ON 0x04   // 开
// 外部时钟设置
#define HY2613_CMD_EXCTL_RTC_OFF 0x00  // 外接时钟
#define HY2613_CMD_EXCTL_RTC_ON 0x02   // 外接振荡器
// 特殊寄存器设置
#define HY2613_CMD_EXCTL_SR_OFF 0x00  // 关
#define HY2613_CMD_EXCTL_SR_ON 0x01   // 开

bool hy2613_init(void);
void hy2613_off(void);
void hy2613_on(void);
void hy2613_refresh(void *lcd_pixel);
void hy2613_test(bool flag);

#endif
