/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021-2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <bfdev/rbtree.h>
#include "py32f0xx_hal.h"

#define TEST_LEN 50
#define TEST_LOOP 10000
static BFDEV_RB_ROOT(bench_root);

struct bench_node {
    struct bfdev_rb_node node;
    unsigned int data;
};

#define rb_to_bench(ptr) \
    bfdev_rb_entry_safe(ptr, struct bench_node, node)

static long demo_cmp(const struct bfdev_rb_node *a, const struct bfdev_rb_node *b)
{
    struct bench_node *demo_a = rb_to_bench(a);
    struct bench_node *demo_b = rb_to_bench(b);
    return demo_a->data - demo_b->data;
}

int benchmark(void)
{
    struct bench_node *node;
    unsigned int count, loop;
    uint32_t start, time;
    void *block;

    node = block = malloc(sizeof(*node) * TEST_LEN);
    if (!block) {
        printf("Insufficient Memory!\n");
        return -ENOMEM;
    }

    printf("Generate Node: %u\n", TEST_LEN);
    for (count = 0; count < TEST_LEN; ++count)
        node[count].data = count;

    printf("Insert Nodes: %u\n", TEST_LOOP * TEST_LEN);
    start = HAL_GetTick();

    for (loop = 0; loop < TEST_LOOP; ++loop) {
        for (count = 0; count < TEST_LEN; ++count)
            bfdev_rb_insert(&bench_root, &node[count].node, demo_cmp);
        bench_root = BFDEV_RB_INIT;
    }

    time = HAL_GetTick() - start;
    printf("Total time: %lu.%lus\n", time / 1000, time % 1000);
    free(block);

    return 0;
}
