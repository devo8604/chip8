#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#include "config.h"

struct chip8;
struct stack
{
    uint16_t stack[CHIP8_STACK_SIZE];
};

void stack_push(struct chip8 *chip8, uint16_t val);
uint16_t stack_pop(struct chip8 *chip8);

#endif