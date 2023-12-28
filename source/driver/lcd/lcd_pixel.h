#ifndef __LCD_PIXEL_H__
#define __LCD_PIXEL_H__

#include <stdint.h>
#include "cmsis_compiler.h"

typedef __PACKED struct _lcd_pixel_t
{
    // 0x00 - 0x0F
    uint32_t p_min_p : 1; //
    uint32_t s_39 : 1;
    uint32_t s_40 : 1;
    uint32_t s_41 : 1;
    uint32_t p_min_min : 1; //
    uint32_t s_ol : 1;
    uint32_t s_43 : 1;
    uint32_t s_42 : 1;
    uint32_t s_32 : 1;
    uint32_t s_33 : 1;
    uint32_t s_34 : 1;
    uint32_t s_35 : 1;
    uint32_t p_max_max : 1; //
    uint32_t s_38 : 1;
    uint32_t s_37 : 1;
    uint32_t s_36 : 1;
    // 0x10 - 0x1F
    uint32_t delta : 1;
    uint32_t s_26 : 1;
    uint32_t s_27 : 1;
    uint32_t s_28 : 1;
    uint32_t p_max_p : 1; //
    uint32_t s_31 : 1;
    uint32_t s_30 : 1;
    uint32_t s_29 : 1;
    uint32_t s_18 : 1;
    uint32_t s_19 : 1;
    uint32_t s_20 : 1;
    uint32_t s_21 : 1;
    uint32_t s_25 : 1;
    uint32_t s_24 : 1;
    uint32_t s_23 : 1;
    uint32_t s_22 : 1;
    // 0x20 - 0x2F
    uint32_t s_11 : 1;
    uint32_t s_12 : 1;
    uint32_t s_13 : 1;
    uint32_t s_14 : 1;
    uint32_t ncv : 1; // NCV非接触测量
    uint32_t s_17 : 1;
    uint32_t s_16 : 1;
    uint32_t s_15 : 1;
    uint32_t s_3 : 1;
    uint32_t s_4 : 1;
    uint32_t s_5 : 1;
    uint32_t s_6 : 1;
    uint32_t s_10 : 1;
    uint32_t s_9 : 1;
    uint32_t s_8 : 1;
    uint32_t s_7 : 1;
    // 0x30 - 0x3F
    uint32_t time : 1;  // 倒计时
    uint32_t hold : 1;  // HOLD
    uint32_t dc : 1;    // 直流
    uint32_t minus : 1; // 负号
    uint32_t s_2 : 1;
    uint32_t s_1 : 1;
    uint32_t s_minus : 1;   // 模拟条负号
    uint32_t s_scale : 1;   // 模拟条刻度
    uint32_t buzzer : 1;    // 蜂鸣器
    uint32_t diode : 1;     // 二极管
    uint32_t hfe : 1;       // 三极管测量
    uint32_t low_bat : 1;   // 电池电压低
    uint32_t t_rms : 1;     // 有效值标志T-RMS
    uint32_t lightning : 1; // 闪电高压标志
    uint32_t serial : 1;    // 串口连接[S]
    uint32_t ac : 1;        // 交流AC
    // 0x40 - 0x4F
    uint32_t thousand : 1; // 千k
    uint32_t omen : 1;     // 兆M
    uint32_t ampere : 1;   // 安培A
    uint32_t volt : 1;     // 伏V
    uint32_t nanon : 1;    // 纳
    uint32_t micron : 1;   // 微u
    uint32_t milli : 1;    // 毫m
    uint32_t farad : 1;    // 法拉
    uint32_t auto_off : 1; // auto符号
    uint32_t digitron_4_e : 1;
    uint32_t digitron_4_f : 1;
    uint32_t digitron_4_a : 1;
    uint32_t ohm : 1;     // 欧姆
    uint32_t hz : 1;      // 赫兹
    uint32_t percent : 1; // %百分号
    uint32_t beta : 1;    // 相对符号
    // 0x50 - 0x5F
    uint32_t digitron_4_dp : 1;
    uint32_t digitron_3_e : 1;
    uint32_t digitron_3_f : 1;
    uint32_t digitron_3_a : 1;
    uint32_t digitron_4_d : 1;
    uint32_t digitron_4_c : 1;
    uint32_t digitron_4_g : 1;
    uint32_t digitron_4_b : 1;
    uint32_t digitron_3_dp : 1;
    uint32_t digitron_2_e : 1;
    uint32_t digitron_2_f : 1;
    uint32_t digitron_2_a : 1;
    uint32_t digitron_3_d : 1;
    uint32_t digitron_3_c : 1;
    uint32_t digitron_3_g : 1;
    uint32_t digitron_3_b : 1;
    // 0x60 - 0x6F
    uint32_t digitron_2_dp : 1;
    uint32_t digitron_1_e : 1;
    uint32_t digitron_1_f : 1;
    uint32_t digitron_1_a : 1;
    uint32_t digitron_2_d : 1;
    uint32_t digitron_2_c : 1;
    uint32_t digitron_2_g : 1;
    uint32_t digitron_2_b : 1;
    uint32_t digitron_1_dp : 1;
    uint32_t digitron_0_e : 1;
    uint32_t digitron_0_f : 1;
    uint32_t digitron_0_a : 1;
    uint32_t digitron_1_d : 1;
    uint32_t digitron_1_c : 1;
    uint32_t digitron_1_g : 1;
    uint32_t digitron_1_b : 1;
    // 0x70 - 0x7F
    uint32_t reserve1 : 1;
    uint32_t reserve2 : 1;
    uint32_t reserve3 : 1;
    uint32_t low_pass : 1; // 低通符号
    uint32_t digitron_0_d : 1;
    uint32_t digitron_0_c : 1;
    uint32_t digitron_0_g : 1;
    uint32_t digitron_0_b : 1;
    uint32_t reserve4 : 1;
    uint32_t reserve5 : 1;
    uint32_t reserve6 : 1;
    uint32_t reserve7 : 1;
    uint32_t reserve8 : 1;
    uint32_t reserve9 : 1;
    uint32_t reserve10 : 1;
    uint32_t reserve11 : 1;
} lcd_pixel_t;

#endif
