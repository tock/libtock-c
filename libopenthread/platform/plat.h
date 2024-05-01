#pragma once
#include<stdbool.h>

typedef struct {
    char *buffer;
    int write_index;
    int read_index;
    bool new;
} ring_buffer;
