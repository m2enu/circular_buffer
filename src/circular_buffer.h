/**
 * @file        circular_buffer.h
 * @brief       Circular buffer for embedded systems.
 * @author      m2enu
 * @date        2021/05/05
 * @copyright   Copyright (c) 2021 m2enu
 * @par         License
 *              https://github.com/m2enu/circular_buffer/blob/main/LICENSE
 */
#ifndef CIRCULAR_BUFFER_H_INCLUDED
#define CIRCULAR_BUFFER_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

/**
 * @brief       Circular buffer size definition
 */
#define CIRCULAR_BUFFER_SIZE        (128)

/**
 * @brief       Mask pattern for circular buffer index
 */
#define CIRCULAR_BUFFER_SIZE_MASK   (CIRCULAR_BUFFER_SIZE - 1)

#if (CIRCULAR_BUFFER_SIZE & CIRCULAR_BUFFER_SIZE_MASK) != 0
#error "CIRCULAR_BUFFER_SIZE must be a power of 2."
#endif

/**
 * @brief       Circular buffer structure
 */
typedef struct circular_buffer_t_tag
{
    /** Entity of buffer */
    uint8_t     buffer[CIRCULAR_BUFFER_SIZE];
    /** Pushed size */
    size_t      enqueue;
    /** Size that will be poped */
    size_t      dequeue;
} circular_buffer_t;

/**
 * @brief       Get pushed data count of specified buffer
 * @param[in]   p_buf   Pointer to circular buffer
 * @return      Pushed data count
 */
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

/**
 * @brief       Return true if specified buffer is available (able to push)
 * @param[in]   p_buf   Pointer to circular buffer
 * @return      The buffer is available or not
 * @retval      (true)  The buffer is available
 * @retval      (false) The buffer is not available
 */
static inline size_t circular_buffer_available(
    const circular_buffer_t * p_buf)
{
    return (CIRCULAR_BUFFER_SIZE - circular_buffer_count(p_buf));
}

/**
 * @brief       Return true if specified buffer is empty
 * @param[in]   p_buf   Pointer to circular buffer
 * @return      The buffer is empty or not
 * @retval      (true)  The buffer is empty
 * @retval      (false) The buffer is not empty
 */
static inline bool circular_buffer_is_empty(
    const circular_buffer_t * p_buf)
{
    return (circular_buffer_count(p_buf) == 0);
}

/**
 * @brief       Return true if specified buffer is full
 * @param[in]   p_buf   Pointer to circular buffer
 * @return      The buffer is full or not
 * @retval      (true)  The buffer is full
 * @retval      (false) The buffer is not full
 */
static inline bool circular_buffer_is_full(
    const circular_buffer_t * p_buf)
{
    return (circular_buffer_available(p_buf) == 0);
}

/**
 * @brief       Initialize circular buffer
 * @param[out]  p_buf   Pointer to circular buffer
 * @return      nothing
 */
void circular_buffer_init(circular_buffer_t * p_buf);

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
    const uint8_t * data,
    size_t length);

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
    size_t length);

#endif
