#include <stdarg.h>
#include "ulog.h"
#include "printf.h"
#include "irda.h"

static ulog_level_t log_level = ulog_level_user;

static char *log_level_type[] =
    {
        "[USER  ] ",
        "[ERROR ] ",
        "[WARN  ] ",
        "[DEBUG ] ",
};

void ulog_set_level(ulog_level_t level)
{
    log_level = level;
}

void ulog_printf(ulog_level_t level, char const *const format, ...)
{
    if (level <= log_level)
    {
        if ((level < 0) || (level >= ulog_level_max))
            return;

        printf(log_level_type[level]);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

void _putchar(char character)
{
    irda_send_byte(character);
}
