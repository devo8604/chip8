#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "config.h"

struct keyboard
{
    bool keyboard[CHIP8_KEYBOARD_KEYTOTAL];
    const char *keyboard_map;
};

void keyboard_map_set(struct keyboard *keyboard, const char *map);
int keyboard_map(const char *map, char key);
bool keyboard_key_isdown(struct keyboard *keyboard, int key);
void keyboard_key_up(struct keyboard *keyboard, int key);
void keyboard_key_down(struct keyboard *keyboard, int key);

#endif