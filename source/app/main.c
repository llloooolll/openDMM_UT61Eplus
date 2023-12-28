#include "qpn.h"
#include "bsp.h"
#include "ao_meter.h"
#include "ao_lcd.h"

static QEvt l_meter_queue[8];
static QEvt l_lcd_queue[8];

QActiveCB const Q_ROM QF_active[] = {
    {(QActive *)0, (QEvt *)0, 0U},
    {(QActive *)&ao_meter, l_meter_queue, Q_DIM(l_meter_queue)},
    {(QActive *)&ao_lcd, l_lcd_queue, Q_DIM(l_lcd_queue)},
};

int main(void)
{
    ao_meter_ctor();
    ao_lcd_ctor();

    QF_init(Q_DIM(QF_active));
    bsp_init();

    return QF_run(); // 开始调度
}
