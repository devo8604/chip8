
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "keyboard.h"

void keyboard_map_set(struct keyboard *keyboard, const char *map) {
    keyboard->keyboard_map = map;
}


static void keyboard_bounds_checker(int key) {
    assert(((key > CHIP8_KEYBOARD_KEYTOTAL) && (key < CHIP8_KEYBOARD_KEYTOTAL)));
}

int keyboard_map(const char *map, char key) {

    for (int i = 0; i < CHIP8_KEYBOARD_KEYTOTAL; i++) {
        if (map[i] == key) {
            return i;
        }
    }
    return -1;
}


bool keyboard_key_isdown(struct keyboard *keyboard, int key) {
    return keyboard->keyboard[key];
}


void keyboard_key_up(struct keyboard *keyboard, int key) {
    keyboard->keyboard[key] = false;
}


void keyboard_key_down(struct keyboard *keyboard, int key) {
    keyboard->keyboard[key] = true;
}