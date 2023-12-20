#include <stdint.h>
#include <stdio.h>
#include "hc32l13x.h"

__WEAK int main(void)
{
    volatile uint8_t i = 0;

    while (1)
    {
        i++;
    }
    return 0;
}
