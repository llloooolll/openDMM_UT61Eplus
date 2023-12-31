#include "ulog.h"
#include "printf.h"
#include "irda.h"

static ulog_level_t _log_level = ulog_level_user;
static uint32_t _log_index = 0;

static char *log_level_type[] =
    {
        "[USER  ] ",
        "[ERROR ] ",
        "[WARN  ] ",
        "[DEBUG ] ",
};

void ulog_set_level(ulog_level_t level)
{
    _log_level = level;
}

void ulog_printf(ulog_level_t level, char const *const format, ...)
{
    if (level <= _log_level)
    {
        if ((level < 0) || (level >= ulog_level_max))
            return;

        printf("%d %s", _log_index++, log_level_type[level]);

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
