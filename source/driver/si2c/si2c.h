#ifndef __SI2C_H__
#define __SI2C_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum _si2c_status_t
{
    si2c_status_ok,        // 正常
    si2c_status_addr_nack, // 地址没有响应
    si2c_status_data_nack, // 数据没有响应
    si2c_status_shortcut,  // 线路异常拉低
    si2c_status_other,     // 其他
} si2c_status_t;

typedef struct _si2c_pin_t
{
    bool (*si2c_scl_option)(bool flag);
    bool (*si2c_sda_option)(bool flag);
} si2c_pin_t;

void si2c_init(void);

#endif
