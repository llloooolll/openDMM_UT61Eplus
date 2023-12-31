#include "ao_meter.h"
#include "ulog.h"
#include "eeprom.h"
#include "ao_lcd.h"
#include "ao_es232.h"
#include "ao_irda.h"

static const uint8_t mode_config[][4] = {
    {0x09, 0x43, 0x80, 0x00}, // ACV
    {0x01, 0x40, 0x00, 0x00}, // DCV
    {0x50, 0x43, 0x80, 0x01}, // MV AC
    {0x48, 0x00, 0x00, 0x00}, // MV  DC
    {0x20, 0x40, 0x00, 0x00}, // OM 电阻
    {0xA8, 0x40, 0x00, 0x00}, // OM 蜂鸣
    {0x30, 0x40, 0x00, 0x00}, // OM 二极管
    {0x40, 0x40, 0x00, 0x00}, // OM 电容
    {0x38, 0x40, 0x00, 0x00}, // HZ 频率
    {0x38, 0x41, 0x00, 0x00}, // HZ 占空比
    {0x48, 0x00, 0x00, 0x00}, // hfe
    {0x10, 0x40, 0x00, 0x00}, // UA DC
    {0x18, 0x43, 0x80, 0x00}, // UA  AC
    {0x10, 0x40, 0x00, 0x00}, // MA DC
    {0x18, 0x43, 0x80, 0x00}, // MA AC
    {0x11, 0x40, 0x00, 0x00}, // A DC
    {0x19, 0x43, 0x80, 0x00}, // A AC
    {0x57, 0x43, 0x80, 0x01}, // NCV
};

// 对象
ao_meter_t ao_meter;

// 状态机
static QState ao_meter_init(ao_meter_t *const me);
static QState ao_meter_idle(ao_meter_t *const me);
static QState ao_meter_active(ao_meter_t *const me);

// 构造
void ao_meter_ctor(void)
{
    ao_meter_t *const me = &ao_meter;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_meter_init));
}

static QState ao_meter_init(ao_meter_t *const me)
{
    me->ready_count = 0;
    me->mode = meter_mode_acv;
    me->mode_config = (es232_write_cmd_t *)&mode_config[0][0];
    return Q_TRAN(&ao_meter_idle);
}

static QState ao_meter_idle(ao_meter_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG:
    {
        status = Q_HANDLED();
        break;
    }
    case AO_METER_READY_SIG:
    {
        if (Q_PAR(me) == 0)
        {
            me->ready_count++;
            if (me->ready_count >= 3)
            {
                ULOG_DEBUG("all hardware done\n");
                ULOG_DEBUG("build: %s %s\n", __DATE__, __TIME__);
                if (eeprom_read_byte(0x00) == 0x00)
                {
                    ULOG_DEBUG("eeprom calibration value exit\n");
                    eeprom_read_all(&me->eeprom[0]);
                }
                QACTIVE_POST(&ao_es232, AO_ES232_ACTIVE_SIG, 0U);
                QACTIVE_POST(&ao_lcd, AO_LCD_ACTIVE_SIG, 0U);
                status = Q_TRAN(&ao_meter_active);
                break;
            }
        }
        else
        {
            ULOG_ERROR("hardware error\n");
        }
        status = Q_HANDLED();
        break;
    }
    default:
    {
        status = Q_SUPER(&QHsm_top);
        break;
    }
    }
    return status;
}

static QState ao_meter_active(ao_meter_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case AO_METER_ADC_DONE_SIG:
    {
        ULOG_DEBUG("get new data\n");
        switch (me->mode)
        {
        case meter_mode_dcv:
            break;
        default:
            break;
        }
        status = Q_HANDLED();
        break;
    }
    case AO_METER_KEY_SIG:
    {
        status = Q_HANDLED();
        break;
    }
    default:
    {
        status = Q_SUPER(&QHsm_top);
        break;
    }
    }
    return status;
}
