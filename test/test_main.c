/**
 * @file        test_main.c
 * @brief       Unit-test for Circular Buffer
 * @author      m2enu
 * @date        2021/05/01
 * @copyright   Copyright (c) 2021 m2enu
 * @par         License
 *              https://github.com/m2enu/CircularBuffer/blob/main/LICENSE
 */
#include <unity.h>
#include <stdint.h>
#include <stdlib.h>
#include "circular_buffer.h"

/**
 * @brief       Get specified array size
 */
#define ARRAY_SIZE(x)   (sizeof(x) / sizeof(x[0]))

/**
 * @brief       Unit test target
 */
static circular_buffer_t tgt;

/**
 * @brief       Queue buffer
 */
static uint8_t src[512];

/**
 * @brief       Dequeue buffer
 */
static uint8_t dst[512];

/**
 * @brief       Executing unit test by specified parameter
 */
static void exec_test(size_t n_src, bool will_be_full)
{
    const size_t len = (CIRCULAR_BUFFER_SIZE < n_src) ? CIRCULAR_BUFFER_SIZE: n_src;
    for (int i = 0; i < n_src; i++)
    {
        src[i] = (uint8_t)i;
    }
    // Initialize
    circular_buffer_init(&tgt);
    // Enqueue
    TEST_ASSERT_EQUAL(false, circular_buffer_enqueue(&tgt, src, n_src));
    // Check item count
    TEST_ASSERT_EQUAL(len, circular_buffer_count(&tgt));
    TEST_ASSERT_EQUAL(CIRCULAR_BUFFER_SIZE - len, circular_buffer_available(&tgt));
    TEST_ASSERT_EQUAL(false, circular_buffer_is_empty(&tgt));
    TEST_ASSERT_EQUAL(will_be_full, circular_buffer_is_full(&tgt));
    // Dequeue
    TEST_ASSERT_EQUAL(false, circular_buffer_dequeue(&tgt, dst, n_src));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(src, dst, len);
    // Check item count
    TEST_ASSERT_EQUAL(0, circular_buffer_count(&tgt));
    TEST_ASSERT_EQUAL(CIRCULAR_BUFFER_SIZE, circular_buffer_available(&tgt));
    TEST_ASSERT_EQUAL(true, circular_buffer_is_empty(&tgt));
    TEST_ASSERT_EQUAL(false, circular_buffer_is_full(&tgt));
    // Unable to dequeue due to buffer is empty
    TEST_ASSERT_EQUAL(true, circular_buffer_dequeue(&tgt, dst, 1));
}

/**
 * @brief       Initializing test
 */
static void test_circular_buffer_init(void)
{
    circular_buffer_init(&tgt);
    TEST_ASSERT_EQUAL(0, tgt.enqueue);
    TEST_ASSERT_EQUAL(0, tgt.dequeue);
    TEST_ASSERT_EQUAL(0, circular_buffer_count(&tgt));
    TEST_ASSERT_EQUAL(128, circular_buffer_available(&tgt));
    TEST_ASSERT_EQUAL(true, circular_buffer_is_empty(&tgt));
    TEST_ASSERT_EQUAL(false, circular_buffer_is_full(&tgt));
}

static void test_circular_buffer_push_less(void)
{
    exec_test(1, false);
}

static void test_circular_buffer_push_just(void)
{
    exec_test(128, true);
}

static void test_circular_buffer_push_over(void)
{
    exec_test(129, true);
}

static void test_circular_buffer_push_2times(void)
{
    const uint8_t src[] = {
        0x11U, 0x22U, 0x33U, 0x44U, 0x55U, 0x66U, 0x77U, 0x88U};
    circular_buffer_init(&tgt);
    // Enqueue
    TEST_ASSERT_EQUAL(false, circular_buffer_enqueue(&tgt, src + 0, 3));
    TEST_ASSERT_EQUAL(false, circular_buffer_enqueue(&tgt, src + 3, 5));
    TEST_ASSERT_EQUAL(  8, circular_buffer_count(&tgt));
    TEST_ASSERT_EQUAL(120, circular_buffer_available(&tgt));
    // Dequeue
    TEST_ASSERT_EQUAL(false, circular_buffer_dequeue(&tgt, dst, 8));
    // Verify
    TEST_ASSERT_EQUAL(true, circular_buffer_is_empty(&tgt));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(src, dst, 8);
}

static void test_circular_buffer_push_wraparound(void)
{
    exec_test(126, false);
    const uint8_t src[] = {0x11U, 0x22U, 0x33U, 0x44U, 0x55U};
    // Enqueue
    TEST_ASSERT_EQUAL(false, circular_buffer_enqueue(&tgt, src, 5));
    TEST_ASSERT_EQUAL(  5, circular_buffer_count(&tgt));
    TEST_ASSERT_EQUAL(123, circular_buffer_available(&tgt));
    // Dequeue
    TEST_ASSERT_EQUAL(false, circular_buffer_dequeue(&tgt, dst, 5));
    // Verify
    TEST_ASSERT_EQUAL(true, circular_buffer_is_empty(&tgt));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(src, dst, 5);
}

/**
 * @brief       Unit test main
 */
int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_circular_buffer_init                  );
    RUN_TEST(test_circular_buffer_push_less             );
    RUN_TEST(test_circular_buffer_push_just             );
    RUN_TEST(test_circular_buffer_push_over             );
    RUN_TEST(test_circular_buffer_push_2times           );
    RUN_TEST(test_circular_buffer_push_wraparound       );
    return UNITY_END();
}

