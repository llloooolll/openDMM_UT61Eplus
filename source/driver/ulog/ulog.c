#include "ulog.h"

#include "irda.h"
#include "printf.h"

#define SHELL_COLOR_ENABLE 1

static ulog_level_t _log_level = ulog_level_error;
static uint32_t _log_index = 0;

static char *log_level_type[] = {
    "[ERROR ] ",  // 错误
    "[WARN  ] ",  //
    "[INFO  ] ",  //
    "[DEBUG ] ",  //
};

static uint8_t log_level_color[] = {
    shell_color_red,     // ERROR
    shell_color_yellow,  // WARN
    shell_color_white,   // INFO
    shell_color_cyan,    // DEBUG
};

/**
 * @brief 设置日志等级
 *
 * @param level
 */
void ulog_set_level(ulog_level_t level) {
    _log_level = level;  //
}

/**
 * @brief 打印log
 *
 * @param level
 * @param format
 * @param ...
 */
void ulog_printf(ulog_level_t level, char const *const format, ...) {
    if ((level < 0) || (level >= ulog_level_max))  //
        return;
    if (level <= _log_level) {
#if SHELL_COLOR_ENABLE
        printf("\033[%dm", log_level_color[level]);
#endif
        printf("%d %s", _log_index++, log_level_type[level]);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
#if SHELL_COLOR_ENABLE
        printf("\033[0m");
#endif
    }
}

/**
 * @brief 清除命令行屏幕
 *
 */
void ulog_clean(void) {
#if SHELL_COLOR_ENABLE
    printf("\033[2J");
#endif
}

/**
 * @brief 打印字符重定向
 *
 * @param character
 */
void _putchar(char character) {
    irda_send_byte(character);  //
}
