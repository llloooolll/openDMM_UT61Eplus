#include "buffer.h"

void buffer_init(buffer_t *buffer) { buffer_clear(buffer); }

uint32_t buffer_can_put(buffer_t *buffer) {
    if (buffer->buffer_put < buffer->buffer_size) {
        return (buffer->buffer_size - buffer->buffer_put);
    }
    return 0;
}

uint32_t buffer_can_get(buffer_t *buffer) {
    if (buffer->buffer_put > buffer->buffer_get) {
        return (buffer->buffer_put - buffer->buffer_get);
    }
    return 0;
}

void buffer_put(buffer_t *buffer, uint8_t data) {
    if (buffer_can_put(buffer) > 0) {
        buffer->buffer_pointer[buffer->buffer_put] = data;
        buffer->buffer_put++;
    }

    if (buffer_can_get(buffer) == 0) {
        buffer_clear(buffer);
    }
}

uint8_t buffer_get(buffer_t *buffer) {
    if (buffer_can_get(buffer) > 0) {
        uint8_t data = buffer->buffer_pointer[buffer->buffer_get];
        buffer->buffer_get++;
        return data;
    }
    return 0;
}

void buffer_clear(buffer_t *buffer) {
    buffer->buffer_get = 0;
    buffer->buffer_put = 0;
}
