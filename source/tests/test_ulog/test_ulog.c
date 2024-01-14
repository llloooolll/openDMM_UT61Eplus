#include "hc32l13x.h"
#include "io_config.h"
#include "irda.h"
#include "sysctrl.h"
#include "ulog.h"

int main(void) {
    sysctrl_set_rch_trim(sysctrl_rch_freq_24M);
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);

    irda_init(9600);

    ulog_set_level(ulog_level_debug);
    ULOG_ERROR("level = debug\r\n");
    ULOG_WARN("level = debug\r\n");
    ULOG_INFO("level = debug\r\n");
    ULOG_DEBUG("level = debug\r\n");

    ulog_set_level(ulog_level_info);
    ULOG_ERROR("level = info\r\n");
    ULOG_WARN("level = info\r\n");
    ULOG_INFO("level = info\r\n");
    ULOG_DEBUG("level = info\r\n");

    ulog_set_level(ulog_level_warn);
    ULOG_ERROR("level = warn\r\n");
    ULOG_WARN("level = warn\r\n");
    ULOG_INFO("level = warn\r\n");
    ULOG_DEBUG("level = warn\r\n");

    ulog_set_level(ulog_level_error);
    ULOG_ERROR("level = error\r\n");
    ULOG_WARN("level = error\r\n");
    ULOG_INFO("level = error\r\n");
    ULOG_DEBUG("level = error\r\n");

    while (1) {
    }
    return 0;
}
