#ifndef CIRCULAR_BUFFER_H_INCLUDED
#define CIRCULAR_BUFFER_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define CIRCULAR_BUFFER_SIZE        (128)
#define CIRCULAR_BUFFER_SIZE_MASK   (CIRCULAR_BUFFER_SIZE - 1)

#if (CIRCULAR_BUFFER_SIZE & CIRCULAR_BUFFER_SIZE_MASK) != 0
#error "CIRCULAR_BUFFER_SIZE must be a power of 2."
#endif

typedef struct circular_buffer_t_tag
{
    uint8_t     buffer[CIRCULAR_BUFFER_SIZE];
    size_t      enqueue;
    size_t      dequeue;
} circular_buffer_t;

static inline size_t circular_buffer_count(
    const circular_buffer_t * p_buf)
{
    if (p_buf == NULL)
    {
        return 0;
    }
    else if (p_buf->dequeue >= p_buf->enqueue)
    {
        return p_buf->dequeue - p_buf->enqueue;
    }
    return p_buf->enqueue - p_buf->dequeue;
}

static inline size_t circular_buffer_available(
    const circular_buffer_t * p_buf)
{
    return (CIRCULAR_BUFFER_SIZE - circular_buffer_count(p_buf));
}

static inline bool circular_buffer_is_empty(
    const circular_buffer_t * p_buf)
{
    return (circular_buffer_count(p_buf) == 0);
}

static inline bool circular_buffer_is_full(
    const circular_buffer_t * p_buf)
{
    return (circular_buffer_available(p_buf) == 0);
}

void circular_buffer_init(circular_buffer_t * p_buf);

bool circular_buffer_enqueue(
    circular_buffer_t * p_buf,
    const uint8_t * data,
    size_t length);

bool circular_buffer_dequeue(
    circular_buffer_t * p_buf,
    uint8_t * p_data,
    size_t length);

#endif
