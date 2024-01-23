#include "ao_es232.h"
#include "ao_irda.h"
#include "ao_knob.h"
#include "ao_lcd.h"
#include "ao_meter.h"
#include "app_button.h"
#include "app_config.h"
#include "app_knob.h"
#include "binary.h"
#include "es232.h"
#include "gpio.h"
#include "io_config.h"
#include "irda.h"
#include "lcd_pixel.h"
#include "qpn.h"
#include "qpn_bsp.h"
#include "ulog.h"

static QEvt l_meter_queue[16];
static QEvt l_lcd_queue[16];
static QEvt l_es232_queue[16];
static QEvt l_irda_queue[16];
static QEvt l_knob_queue[16];

QActiveCB const Q_ROM QF_active[] = {
    {(QActive *)0, (QEvt *)0, 0U},
    {(QActive *)&ao_meter, l_meter_queue, Q_DIM(l_meter_queue)},
    {(QActive *)&ao_es232, l_es232_queue, Q_DIM(l_es232_queue)},
    {(QActive *)&ao_lcd, l_lcd_queue, Q_DIM(l_lcd_queue)},
    {(QActive *)&ao_irda, l_irda_queue, Q_DIM(l_irda_queue)},
    {(QActive *)&ao_knob, l_knob_queue, Q_DIM(l_knob_queue)},
};

int main(void) {
    ao_meter_ctor();
    ao_es232_ctor();
    ao_lcd_ctor();
    ao_irda_ctor();
    ao_knob_ctor();

    QF_init(Q_DIM(QF_active));
    bsp_init();
    app_button_init();
    app_knob_init();
    app_config_reset();
    app_config_read();

    if (!gpio_read_pin(KEY_SELECT_PORT, KEY_SELECT_PIN)) {
        // 按住SELECT关闭自动休眠
        glob_config.glob_auto_sleep_enable = 0;
    }

    irda_init(glob_config.irda_baudrate);
    if (irda_is_exist()) {
        ulog_set_level(ulog_level_debug);
        ulog_clean();  // 滚动屏幕，清除可视区域
    }

    return QF_run();  // 开始调度
}
