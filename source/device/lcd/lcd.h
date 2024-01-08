#ifndef __LCD_H__
#define __LCD_H__

#include "hc32l13x.h"
#include "lcd_pixel.h"

bool lcd_init(void);
void lcd_enable(bool flag);
void lcd_refresh(lcd_pixel_t *lcd_pixel);
void lcd_test(bool flag);
void lcd_show_char(lcd_pixel_t *lcd_pixel, uint8_t index, char value);
void lcd_show_point(lcd_pixel_t *lcd_pixel, uint8_t index, bool flag);
void lcd_show_value(lcd_pixel_t *lcd_pixel, int32_t Value, int8_t u8Power);
void lcd_enable_bl(bool flag);
void lcd_show_scale(lcd_pixel_t *lcd_pixel, int32_t Value);
void lcd_set_ol_threshold(int32_t value);

#endif
