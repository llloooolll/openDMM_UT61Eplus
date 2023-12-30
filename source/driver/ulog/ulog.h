#ifndef __ULOG_H__
#define __ULOG_H__

typedef enum _ulog_level_t
{
    ulog_level_user,
    ulog_level_error,
    ulog_level_warn,
    ulog_level_debug,
    ulog_level_max,
} ulog_level_t;

void ulog_set_level(ulog_level_t level);
void ulog_printf(ulog_level_t level, char const *const format, ...);

#define ULOG_USER(...) ulog_printf(ulog_level_user, __VA_ARGS__)
#define ULOG_ERROR(...) ulog_printf(ulog_level_error, __VA_ARGS__)
#define ULOG_WARN(...) ulog_printf(ulog_level_warn, __VA_ARGS__)
#define ULOG_DEBUG(...) ulog_printf(ulog_level_debug, __VA_ARGS__)

#endif