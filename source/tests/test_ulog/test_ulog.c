#include "hc32l13x.h"
#include "sysctrl.h"
#include "ulog.h"
#include "io_config.h"

int main(void)
{
    sysctrl_set_rch_trim(sysctrl_rch_freq_24M);
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);

    irda_init(9600);

    ulog_set_level(ulog_level_debug);
    ULOG_USER("level = debug\n");
    ULOG_ERROR("level = debug\n");
    ULOG_WARN("level = debug\n");
    ULOG_DEBUG("level = debug\n");

    ulog_set_level(ulog_level_warn);
    ULOG_USER("level = warn\n");
    ULOG_ERROR("level = warn\n");
    ULOG_WARN("level = warn\n");
    ULOG_DEBUG("level = warn\n");

    ulog_set_level(ulog_level_error);
    ULOG_USER("level = error\n");
    ULOG_ERROR("level = error\n");
    ULOG_WARN("level = error\n");
    ULOG_DEBUG("level = error\n");

    ulog_set_level(ulog_level_user);
    ULOG_USER("level = user\n");
    ULOG_ERROR("level = user\n");
    ULOG_WARN("level = user\n");
    ULOG_DEBUG("level = user\n");

    while (1)
    {
    }
    return 0;
}
