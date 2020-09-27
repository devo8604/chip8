#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#include "config.h"

struct memory {
    uint8_t memory[CHIP8_TOTAL_MEMORY];
};

void memory_set(struct memory *memory, int index, uint8_t val);
uint8_t memory_get(struct memory *memory, int index);
uint16_t memory_fetch_2bytes(struct memory *memory, int index);

#endif