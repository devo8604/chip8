
#include <assert.h>

#include "../include/config.h"
#include "../include/memory.h"

static void memory_bounds_check(int index) {
    assert(index < CHIP8_TOTAL_MEMORY);
}

void memory_set(struct memory *memory, int index, uint8_t val) {
    memory_bounds_check(index);
    memory->memory[index] = val;
}

uint8_t memory_get(struct memory *memory, int index) {
    memory_bounds_check(index);
    return memory->memory[index];
}


uint16_t memory_fetch_2bytes(struct memory *memory, int index) {

    assert((index) < CHIP8_TOTAL_MEMORY);
    uint8_t byte1 = memory->memory[index];
    uint8_t byte2 = memory->memory[index+1];
    return byte1 << 8 | byte2;
}
