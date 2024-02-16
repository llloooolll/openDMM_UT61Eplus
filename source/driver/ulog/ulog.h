#ifndef __ULOG_H__
#define __ULOG_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum _ulog_level_t {
    ulog_level_error,
    ulog_level_warn,
    ulog_level_info,
    ulog_level_debug,
    ulog_level_max,
} ulog_level_t;

enum {
    shell_color_black = 30,   //
    shell_color_red = 31,     //
    shell_color_green = 32,   //
    shell_color_yellow = 33,  //
    shell_color_blue = 34,    //
    shell_color_purple = 35,  //
    shell_color_cyan = 36,    //
    shell_color_white = 37,   //
};

void ulog_enable(bool flag);
void ulog_set_level(ulog_level_t level);
void ulog_printf(ulog_level_t level, char const *const format, ...);
void ulog_clean(void);

#define ULOG_ERROR(...) ulog_printf(ulog_level_error, __VA_ARGS__)
#define ULOG_WARN(...) ulog_printf(ulog_level_warn, __VA_ARGS__)
#define ULOG_INFO(...) ulog_printf(ulog_level_info, __VA_ARGS__)
#define ULOG_DEBUG(...) ulog_printf(ulog_level_debug, __VA_ARGS__)

#endif