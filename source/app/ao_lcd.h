#ifndef __AO_LCD_H__
#define __AO_LCD_H__

#include "qpn.h"
#include "lcd.h"

typedef enum _ao_lcd_signal_t
{
    AO_LCD_READY_SIG = Q_USER_SIG, // 初始化
    AO_LCD_ACTIVE_SIG,             //
    AO_LCD_REFRESH_SIG,            // 刷新

    AO_LCD_MAX_SIG
} ao_lcd_signal_t;

typedef struct _ao_lcd_t
{
    QActive super;
    lcd_pixel_t lcd_pixel_buffer;
} ao_lcd_t;

extern ao_lcd_t ao_lcd;

void ao_lcd_ctor(void);

#endif
