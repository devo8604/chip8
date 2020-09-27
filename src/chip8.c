
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "SDL2/SDL.h"

#include "../include/chip8.h"
#include "../include/config.h"
#include "../include/keyboard.h"
#include "../include/screen.h"

// Each line is a pixel representation of a sprite
const char default_charset[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static void opcode_0xF000(struct chip8 *chip8, uint16_t opcode)
{

    uint8_t x = (opcode >> 8) & 0x000F;
    uint8_t kk = opcode & 0x00FF;

    switch (kk)
    {
    case 0x07:
        chip8->registers.V[x] = chip8->registers.delay_timer;
        break;

    case 0x0A:
    {
        SDL_Event event;
        while (SDL_WaitEvent(&event))
        {
            
            if (event.type != SDL_KEYDOWN)
                continue;

            char c = event.key.keysym.sym;
            char chip8_key = keyboard_map(chip8->keyboard.keyboard_map, c);
            if (chip8_key != -1)
            {
                chip8->registers.V[x] = c;
                break;
            }
        }
    }
    break;

    case 0x15:
        chip8->registers.delay_timer = chip8->registers.V[x];
        break;

    case 0x18:
        chip8->registers.sound_timer = chip8->registers.V[x];
        break;

    case 0x1E:
        chip8->registers.I += chip8->registers.V[x];
        break;

    case 0x29:
        chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
        break;

    case 0x33:
    {
        uint8_t hundreds = chip8->registers.V[x] / 100;
        uint8_t tens = (chip8->registers.V[x] / 10) % 10;
        uint8_t units = chip8->registers.V[x] % 10;
        memory_set(&chip8->memory, chip8->registers.I, hundreds);
        memory_set(&chip8->memory, chip8->registers.I + 1, tens);
        memory_set(&chip8->memory, chip8->registers.I + 2, units);
    }
    break;

    case 0x55:
    {
        for (int i = 0; i <= x; i++)
        {
            memory_set(&chip8->memory, (chip8->registers.I + i), chip8->registers.V[i]);
        }
    }
    break;

    case 0x65:
    {
        for (int i = 0; i <= x; i++)
        {
            chip8->registers.V[i] = memory_get(&chip8->memory, chip8->registers.I + i);
        }
    }
    break;
    default:
        assert("Something went realllllly wrong - couldnt find OPCODE!!!!");
    }
}

static void opcode_0x8000(struct chip8 *chip8, uint16_t opcode)
{

    uint8_t n = opcode & 0x000F;
    uint8_t x = (opcode >> 8) & 0x000F;
    uint8_t y = (opcode >> 4) & 0x000F;
    uint16_t tmp = 0;

    switch (n)
    {
    case 0x00:
        chip8->registers.V[x] = chip8->registers.V[y];
        break;

    case 0x01:
        chip8->registers.V[x] = chip8->registers.V[x] | chip8->registers.V[y];
        break;

    case 0x02:
        chip8->registers.V[x] = chip8->registers.V[x] & chip8->registers.V[y];
        break;

    case 0x03:
        chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
        break;

    case 0x04:
    {
        tmp = chip8->registers.V[x] + chip8->registers.V[y];
        chip8->registers.V[0x0F] = 0;

        if (tmp > 0xFF)
        {
            chip8->registers.V[0x0F] = 1;
        }
        chip8->registers.V[x] = tmp;
    }
    break;

    case 0x05:
        chip8->registers.V[0x0F] = 0;
        if (chip8->registers.V[x] > chip8->registers.V[y])
        {
            chip8->registers.V[0x0F] = 1;
        }
        chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
        break;

    case 0x06:
        chip8->registers.V[0x0F] = chip8->registers.V[x] & 0x01;
        chip8->registers.V[x] = chip8->registers.V[x] / 2;
        break;

    case 0x07:
        chip8->registers.V[0x0F] = chip8->registers.V[y] > chip8->registers.V[x];
        chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
        break;

    case 0x0E:
        chip8->registers.V[0x0F] = chip8->registers.V[x] & 0x80;
        chip8->registers.V[x] *= 2;
        break;

    default:
        assert("Something went realllllly wrong - couldnt find OPCODE!!!!");
    }
}

static void chip8_extended_decoder(struct chip8 *chip8, uint16_t opcode)
{

    uint16_t nnn = opcode & 0x0FFF;
    uint8_t n = opcode & 0x000F;
    uint8_t x = (opcode >> 8) & 0x000F;
    uint8_t y = (opcode >> 4) & 0x000F;
    uint8_t kk = opcode & 0x00FF;

    switch (opcode & 0xF000)
    {
    case 0x1000:
        chip8->registers.program_counter = nnn;
        break;

    case 0x2000:
        stack_push(chip8, chip8->registers.program_counter);
        chip8->registers.program_counter = nnn;
        break;

    case 0x3000:
        if (chip8->registers.V[x] == kk)
        {
            chip8->registers.program_counter += 2;
        }
        break;

    case 0x4000:
        if (chip8->registers.V[x] != kk)
        {
            chip8->registers.program_counter += 2;
        }
        break;

    case 0x5000:
        if (chip8->registers.V[x] == chip8->registers.V[y])
        {
            chip8->registers.program_counter += 2;
        }
        break;

    case 0x6000:
        chip8->registers.V[x] = kk;
        break;

    case 0x7000:
        chip8->registers.V[x] = chip8->registers.V[x] + kk;
        break;

    case 0x8000:
        opcode_0x8000(chip8, opcode);
        break;

    case 0x9000:
        if (chip8->registers.V[x] == chip8->registers.V[y])
        {
            chip8->registers.program_counter += 2;
        }

    case 0xA000:
        chip8->registers.I = nnn;
        break;

    case 0xB000:
        chip8->registers.program_counter = nnn + chip8->registers.V[0];
        break;

    case 0xC000:
        chip8->registers.V[x] = (rand() % 0xFF) & kk;
        break;

    case 0xD000:
    {
        chip8->registers.V[0x0F] = screen_draw_sprite(&chip8->screen,
                                                      chip8->registers.V[x],
                                                      chip8->registers.V[y],
                                                      (const char *)&chip8->memory.memory[chip8->registers.I],
                                                      n);
    }
    break;

    case 0xE000:

        switch (n)
        {
        case 0x0E:
        {
            bool isdown = keyboard_key_isdown(&chip8->keyboard,
                                              chip8->registers.V[x]);

            if (isdown)
            {
                chip8->registers.program_counter += 2;
            }
        }
        break;

        case 0x01:
        {
            bool isdown = keyboard_key_isdown(&chip8->keyboard,
                                              chip8->registers.V[x]);

            if (!isdown)
            {
                chip8->registers.program_counter += 2;
            }
        }
        break;
        }
    case 0xF000:
        opcode_0xF000(chip8, opcode);
        break;
    }
}

void chip8_init(struct chip8 *chip8)
{
    memset(chip8, 0, sizeof(struct chip8));

    // Copy the char set into memory
    memcpy(&chip8->memory.memory, default_charset, sizeof(default_charset));
}

void chip8_load(struct chip8 *chip8, const int8_t *buf, size_t size)
{
    assert((size + CHIP8_APP_MEMORY_START) < CHIP8_TOTAL_MEMORY);

    // Load the application to memory addr 0x200
    memcpy(&chip8->memory.memory[CHIP8_APP_MEMORY_START], buf, size);

    // Set the program counter to 0x200
    chip8->registers.program_counter = CHIP8_APP_MEMORY_START;
}

void chip8_exec(struct chip8 *chip8, uint16_t opcode)
{

    switch (opcode)
    {
    case 0x00E0:
        screen_clear(&chip8->screen);
        break;

    case 0x00EE:
        chip8->registers.program_counter = stack_pop(chip8);
        break;

    default:
        chip8_extended_decoder(chip8, opcode);
        break;
    }
}
