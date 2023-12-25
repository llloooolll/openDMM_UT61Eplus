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
void si2c_trans_begin(uint8_t address);
si2c_status_t si2c_trans_end(void);
uint8_t si2c_request_from(uint8_t address, uint8_t size);
void si2c_byte_write(uint8_t data);
void si2c_bytes_write(uint8_t *data, uint8_t size);
int si2c_available(void);
uint8_t si2c_peek(void);
int si2c_byte_read(void);
int HAL_SI2C_bytes_read(uint8_t *buf, uint8_t size);

#endif