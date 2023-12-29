#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>
#include <stdbool.h>
#include "lcd_pixel.h"

bool lcd_init(void);
void lcd_enable(bool flag);
void lcd_refresh(lcd_pixel_t *lcd_pixel);
void lcd_test(bool flag);

#endif
