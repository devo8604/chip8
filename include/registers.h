#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#include "config.h"

struct registers {
    uint8_t V[CHIP8_V_REGISTERS_COUNT];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t stack_pointer;
    uint16_t program_counter;
    uint16_t I; // Used for memory addresses
};

#endif