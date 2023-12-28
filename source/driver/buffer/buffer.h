#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct _buffer_t
{
    uint8_t *buffer_pointer;
    uint32_t buffer_size;
    uint32_t buffer_put;
    uint32_t buffer_get;
} buffer_t;

void buffer_init(buffer_t *buffer);
uint32_t buffer_can_put(buffer_t *buffer);
uint32_t buffer_can_get(buffer_t *buffer);
void buffer_put(buffer_t *buffer, uint8_t data);
uint8_t buffer_get(buffer_t *buffer);
void buffer_clear(buffer_t *buffer);

#endif
