#ifndef __AO_ES232_H__
#define __AO_ES232_H__

#include "es232.h"
#include "qpn.h"

typedef enum _ao_es232_signal_t {
    AO_ES232_READY_SIG = Q_USER_SIG,  // 初始化
    AO_ES232_ACTIVE_SIG,
    AO_ES232_WRITE_CONFIG_SIG,  // 写入配置
    AO_ES232_ENABLE_BUZ_SIG,    // 使能蜂鸣器

    AO_ES232_MAX_SIG
} ao_es232_signal_t;

typedef struct _ao_es232_t {
    QActive super;
    uint32_t es232_read_interval_time;  // es232读取数据间隔
    es232_write_t es232_write_buffer;   //
    es232_read_t es232_read_buffer;     //
} ao_es232_t;

extern ao_es232_t ao_es232;

void ao_es232_ctor(void);

#endif
