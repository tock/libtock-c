#ifndef PLAT_H
#define PLAT_H
#include<stdbool.h>

#define MAX_RING_BUFFER_PACKETS 6;

typedef struct {
    char *buffer;
    int write_index;
    int read_index;
    bool new;
} ring_buffer;

#endif // PLAT_H