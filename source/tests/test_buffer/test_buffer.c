#include "hc32l13x.h"
#include "sysctrl.h"
#include "buffer.h"

#define BUFFER_SIZE 8

static uint8_t test_buffer_data[BUFFER_SIZE];
buffer_t test_buffer = {
    .buffer_pointer = test_buffer_data,
    .buffer_size = BUFFER_SIZE,
};

int main(void)
{
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);
    sysctrl_set_rch_trim(sysctrl_rch_freq_24M);

    buffer_init(&test_buffer);

    for (uint32_t i = 0; i < BUFFER_SIZE; i++)
    {
        if (buffer_can_put(&test_buffer))
        {
            buffer_put(&test_buffer, i);
        }
    }

    for (uint32_t i = 0; i < BUFFER_SIZE; i++)
    {
        if (buffer_can_get(&test_buffer))
        {
            uint8_t data = buffer_get(&test_buffer);
            if (data != i)
            {
                break;
            };
        }
    }

    while (1)
    {
    }
    return 0;
}
