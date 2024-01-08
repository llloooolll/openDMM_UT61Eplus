#include "ao_es232.h"
#include "ao_irda.h"
#include "ao_key.h"
#include "ao_lcd.h"
#include "ao_meter.h"
#include "binary.h"
#include "bsp.h"
#include "es232.h"
#include "lcd_pixel.h"
#include "qpn.h"

static QEvt l_meter_queue[16];
static QEvt l_lcd_queue[16];
static QEvt l_es232_queue[16];
static QEvt l_irda_queue[16];
static QEvt l_key_queue[16];

QActiveCB const Q_ROM QF_active[] = {
    {(QActive *)0, (QEvt *)0, 0U},
    {(QActive *)&ao_meter, l_meter_queue, Q_DIM(l_meter_queue)},
    {(QActive *)&ao_es232, l_es232_queue, Q_DIM(l_es232_queue)},
    {(QActive *)&ao_lcd, l_lcd_queue, Q_DIM(l_lcd_queue)},
    {(QActive *)&ao_irda, l_irda_queue, Q_DIM(l_irda_queue)},
    {(QActive *)&ao_key, l_key_queue, Q_DIM(l_key_queue)},
};

int main(void) {
    ao_meter_ctor();
    ao_es232_ctor();
    ao_lcd_ctor();
    ao_irda_ctor();
    ao_key_ctor();

    QF_init(Q_DIM(QF_active));
    bsp_init();

    return QF_run();  // 开始调度
}
