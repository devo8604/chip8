
#include <assert.h>

#include "../include/chip8.h"
#include "../include/stack.h"

void stack_bounds_checker(int index) {
    assert(index < CHIP8_STACK_SIZE);
}

void stack_push(struct chip8 *chip8, uint16_t val) {
    
    chip8->registers.stack_pointer += 1;
    stack_bounds_checker(chip8->registers.stack_pointer);
    chip8->stack.stack[chip8->registers.stack_pointer] = val;
}


uint16_t stack_pop(struct chip8 *chip8) {

    uint16_t stack_val = chip8->stack.stack[chip8->registers.stack_pointer];
    chip8->registers.stack_pointer -= 1;

    return stack_val;
}