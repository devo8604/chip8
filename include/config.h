#ifndef CONFIG_H
#define CONFIG_H

//==============================================================================
// Misc
//==============================================================================

#define WINDOW_TITLE "Chip8 Emulator"

//==============================================================================
// Keyboard
//==============================================================================

#define CHIP8_KEYBOARD_KEYTOTAL 16

//==============================================================================
// Display
//==============================================================================

#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32

#define CHIP8_DISPLAY_SCALE_FACTOR 10

#define CHIP8_SCALED_DISPLAY_WITDH (CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_SCALE_FACTOR)
#define CHIP8_SCALED_DISPLAY_HEIGHT (CHIP8_DISPLAY_HEIGHT * CHIP8_DISPLAY_SCALE_FACTOR)

#define CHIP8_DEFAULT_SPRITE_HEIGHT 5

//==============================================================================
// Registers
//==============================================================================

#define CHIP8_V_REGISTERS_COUNT 16

//==============================================================================
// Memory
//==============================================================================

#define CHIP8_TOTAL_MEMORY 0xFFF     // 4096
#define CHIP8_APP_MEMORY_START 0x200 // 512
#define CHIP8_CHARACTER_LOAD_ADDR 0x00

//==============================================================================
// Memory
//==============================================================================

#define CHIP8_STACK_SIZE 16

#endif