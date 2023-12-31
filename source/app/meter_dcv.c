#include "meter_dcv.h"
#include "ao_lcd.h"
#include "ulog.h"

QState meter_dcv_adc(ao_meter_t *const me)
{
    QState status;

    int32_t data = es232_get_D0(&me->es232_read_buffer);
    lcd_show_value(&me->lcd_pixel_buffer, data, 4);
    QACTIVE_POST(&ao_lcd, AO_LCD_REFRESH_SIG, (uint32_t)&me->lcd_pixel_buffer);

    status = Q_HANDLED();
    return status;
}

QState meter_dcv_key(ao_meter_t *const me)
{
    QState status;

    status = Q_HANDLED();
    return status;
}
