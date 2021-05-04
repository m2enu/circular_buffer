/**
 * @file        circular_buffer.c
 * @brief       Circular buffer for embedded systems.
 * @author      m2enu
 * @date        2021/05/05
 * @copyright   Copyright (c) 2021 m2enu
 * @par         License
 *              https://github.com/m2enu/circular_buffer/blob/main/LICENSE
 */
#include <stdlib.h>
#include "circular_buffer.h"

static void circular_buffer_memset(uint8_t * dst, uint8_t src, size_t length);
static void circular_buffer_memcpy(uint8_t * dst, const uint8_t * src, size_t length);

/**
 * @brief       Get smaller value between specified values
 * @param[in]   a       Value 1
 * @param[in]   b       Value 2
 * @return      Smaller value
 */
static inline size_t min(size_t a, size_t b)
{
    return (a < b) ? a: b;
}

/**
 * @brief       Convert specified length to circular buffer index.
 * @param[in]   length  Circular buffer index that might be exceeds its size
 * @return      Circular buffer index
 */
static inline size_t buffer_size(size_t length)
{
    return length & CIRCULAR_BUFFER_SIZE_MASK;
}

/**
 * @brief       Set specified value to destination buffer.
 * @param[out]  dst     Pointer to destination buffer
 * @param[in]   src     Value to be set
 * @param[in]   length  Length of setting
 * @return      nothing
 */
static void circular_buffer_memset(uint8_t * dst, uint8_t src, size_t length)
{
    size_t i;
    for (i = 0; i < length; i++)
    {
        *dst++ = src;
    }
}

/**
 * @brief       Copy buffer value from source to destination.
 * @param[out]  dst     Pointer to destination buffer
 * @param[in]   src     Pointer to source buffer
 * @param[in]   length  Length of copying
 * @return      nothing
 */
static void circular_buffer_memcpy(uint8_t * dst, const uint8_t * src, size_t length)
{
    size_t i;
    for (i = 0; i < length; i++)
    {
        *dst++ = *src++;
    }
}

/**
 * @brief       Initialize circular buffer
 * @param[out]  p_buf   Pointer to circular buffer
 * @return      nothing
 */
void circular_buffer_init(circular_buffer_t * p_buf)
{
    if (p_buf == NULL)
    {
        return;
    }
    size_t count = circular_buffer_count(p_buf);
    circular_buffer_memset(p_buf->buffer, 0x00U, count);
    p_buf->enqueue = 0;
    p_buf->dequeue = 0;
}

/**
 * @brief       Push data array to circular buffer
 * @param[in]   p_buf   Pointer to circular buffer
 * @param[in]   p_data  Pointer data to source data array
 * @param[in]   length  Length of `p_data`
 * @return      Success or failed as boolean
 * @retval      (true)  Failed
 * @retval      (false) Succeeded
 */
bool circular_buffer_enqueue(
    circular_buffer_t * p_buf,
    const uint8_t * p_data,
    size_t length)
{
    if (
           (p_buf == NULL)
        || (p_data == NULL)
    ) {
        return true;
    }

    // Decide target length
    length = min(length, circular_buffer_available(p_buf));
    // Push from enqueue index to buffer tail.
    size_t index = buffer_size(p_buf->enqueue);
    size_t c = min(length, CIRCULAR_BUFFER_SIZE - index);
    circular_buffer_memcpy(p_buf->buffer + index, p_data, c);
    // Push remaining data from buffer head.
    circular_buffer_memcpy(p_buf->buffer, p_data + c, length - c);
    p_buf->enqueue += length;

    return false;
}

/**
 * @brief       Pop data array from circular buffer
 * @param[in]   p_buf   Pointer to circular buffer
 * @param[out]  p_data  Pointer data to destination data array
 * @param[in]   length  Length of `p_data`
 * @return      Success or failed as boolean
 * @retval      (true)  Failed
 * @retval      (false) Succeeded
 */
bool circular_buffer_dequeue(
    circular_buffer_t * p_buf,
    uint8_t * p_data,
    size_t length)
{
    if (
           (p_buf == NULL)
        || (p_data == NULL)
        || (circular_buffer_is_empty(p_buf))
    ) {
        return true;
    }

    // Decide target length
    length = min(length, circular_buffer_count(p_buf));
    // Pop from dequeue index to buffer tail.
    size_t index = buffer_size(p_buf->dequeue);
    size_t c = min(length, CIRCULAR_BUFFER_SIZE - index);
    circular_buffer_memcpy(p_data, p_buf->buffer + index, c);
    // Pop remaining data from buffer head.
    circular_buffer_memcpy(p_data + c, p_buf->buffer, length - c);
    p_buf->dequeue += length;
    return false;
}
