#ifndef __HY2613_H__
#define __HY2613_H__

#include "binary.h"
#include "hc32l13x.h"

/* ES232地址 */
#define ES232_I2C_ADDR 0xC8

#define ES232_BUZ_ON 0x02  // 鸣响标志

// static const uint8_t es232_init_config[][4] = {
//     {0x09, 0x43, 0x80, 0x00},  // ACV
//     {0x01, 0x40, 0x00, 0x00},  // DCV
//     {0x50, 0x43, 0x80, 0x01},  // MV AC
//     {0x48, 0x00, 0x00, 0x00},  // MV DC
//     {0x20, 0x40, 0x00, 0x00},  // OHM 电阻
//     {0xA8, 0x40, 0x00, 0x00},  // OHM 蜂鸣
//     {0x30, 0x40, 0x00, 0x00},  // OHM 二极管
//     {0x40, 0x40, 0x00, 0x00},  // OHM 电容
//     {0x38, 0x40, 0x00, 0x00},  // HZ 频率
//     {0x38, 0x41, 0x00, 0x00},  // HZ 占空比
//     {0x48, 0x00, 0x00, 0x00},  // hfe
//     {0x10, 0x40, 0x00, 0x00},  // UA DC
//     {0x18, 0x43, 0x80, 0x00},  // UA AC
//     {0x10, 0x40, 0x00, 0x00},  // MA DC
//     {0x18, 0x43, 0x80, 0x00},  // MA AC
//     {0x11, 0x40, 0x00, 0x00},  // A DC
//     {0x19, 0x43, 0x80, 0x00},  // A AC
//     {0x57, 0x43, 0x80, 0x01},  // NCV
// };

enum es232_mode {
    ES232_MODE_V = B0000,
    ES232_MODE_ACV_HZ = B0001,
    ES232_MODE_A = B0010,
    ES232_MODE_ACA_HZ = B0011,
    ES232_MODE_RES = B0100,
    ES232_MODE_CON = B0101,
    ES232_MODE_DIO = B0110,
    ES232_MODE_F_DUTY = B0111,
    ES232_MODE_CAP = B1000,
    ES232_MODE_ADP = B1001,
    ES232_MODE_ADP_HZ = B1010,
};

enum es232_buzzer_freq {
    F_1_00K = B000,
    F_1_33K = B100,
    F_2_00K = B010,
    F_2_22K = B110,
    F_2_67K = B001,
    F_3_08K = B101,
    F_3_33K = B011,
    F_4_00K = B111,
};

enum es232_range_freq {
    FREQ_30 = B000,
    FREQ_300 = B001,
    FREQ_3K = B010,
    FREQ_30K = B011,
};

enum es232_lpf {
    LPF_0_DIS = B000,
    LPF_1_1K = B001,    // 1K   @3dB
    LPF_2_10K = B010,   // 10K  @3dB
    LPF_3_100K = B011,  // 100K @3dB
};

typedef struct _es232_write_t {
    uint8_t range_msb : 3;   // 量程选择
    uint8_t mode_msb : 4;    // 测量功能控制位
    uint8_t shbp_dcsel : 1;  // 连续性和二极管模式/ AC+DC模式

    uint8_t freq_range_msb : 3;   // 频率量程选择
    uint8_t rev1 : 2;             // NC
    uint8_t buzzer_freq_lsb : 3;  // 蜂鸣器频率选择

    uint8_t rp : 1;       // 交流+直流模式的频率模式电阻控制位
    uint8_t lpf_msb : 2;  // 低通滤波器选择的3dB BW：
    uint8_t fd : 1;       // 30kHz范围内的F+工作模式辅助控制位：FD
    uint8_t ext : 1;      // 二极管模式控制位的外部电源
    uint8_t buf_acdc : 1;  // AC+DC模式的缓冲控制位
    uint8_t bufcal : 1;    // AC+DC模式的缓冲控制位
    uint8_t ac : 1;        // 交流测量模式使能

    uint8_t ext_adp : 1;  // ADP模式
    uint8_t op_lsb : 2;   // 运放控制位
    uint8_t rev2 : 3;     // NC
    uint8_t pcal : 1;     // 校准模式
    uint8_t peak : 1;     // peak模式
} es232_write_t;

typedef struct _es232_read_t {
    /*R1*/
    uint8_t ALARM : 1;  // 超量程警告
    uint8_t STA0 : 1;
    uint8_t BTS1 : 1;   // 电池电压标志
    uint8_t BTS0 : 1;   // 电池电压标志
    uint8_t PMIN : 1;   // 输出的是最小值
    uint8_t PMAX : 1;   // 输出的是最大值
    uint8_t BSIGN : 1;  // FADC负号
    uint8_t ASIGN : 1;  // SADC负号
    /*R2*/
    uint8_t D0_0_2 : 3;
    uint8_t F_FIN : 1;
    uint8_t STA1 : 1;
    uint8_t LDUTY : 1;
    uint8_t LF : 1;
    uint8_t HF : 1;
    /*R3*/
    uint8_t D0_3_10 : 8;
    /*R4*/
    uint8_t D0_11_18 : 8;
    /*R5*/
    uint8_t D1_0_7 : 8;
    /*R6*/
    uint8_t D2_0_5 : 6;
    uint8_t D1_8_9 : 2;
    /*R7*/
    uint8_t D2_6_13 : 8;
    /*R8*/
    uint8_t D3_0_2 : 3;
    uint8_t D2_14_18 : 5;
    /*R9*/
    uint8_t D3_3_10 : 8;
    /*R10*/
    uint8_t D3_11_18 : 8;
} es232_read_t;

void es232_gpio_init(void);
bool es232_init(void);
void es232_enable_power(bool flag);
void es232_write(es232_write_t *es232_write);
void es232_read(es232_read_t *es232_read);
int32_t es232_get_D0(es232_read_t *es232_read_temp);
int32_t es232_get_D1(es232_read_t *es232_read_temp);
bool es232_is_data_ready(void);
void es232_enable_buz(bool flag);

#endif
