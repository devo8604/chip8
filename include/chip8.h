#ifndef CHIP8_H
#define CHIP8_H

#include <stddef.h>

#include "config.h"
#include "keyboard.h"
#include "memory.h"
#include "registers.h"
#include "screen.h"
#include "stack.h"

struct chip8 {
    struct memory memory;
    struct registers registers;
    struct stack stack;
    struct keyboard keyboard;
    struct screen screen;
};

void chip8_init(struct chip8 *chip8);
void chip8_load(struct chip8 *chip8, const int8_t *buf, size_t size);
void chip8_exec(struct chip8 *chip8, uint16_t opcode);

#endif