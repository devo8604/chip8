
#include <assert.h>
#include <string.h>

#include "../include/config.h"
#include "../include/screen.h"

void screen_bounds_checker(int x, int y)
{
    assert(((x < CHIP8_DISPLAY_WIDTH) && (y < CHIP8_DISPLAY_HEIGHT)));
}

void screen_set(struct screen *screen, int x, int y)
{
    screen_bounds_checker(x, y);
    screen->pixels[y][x] = true;
}

bool screen_is_set(struct screen *screen, int x, int y)
{
    screen_bounds_checker(x, y);
    return screen->pixels[y][x];
}

void screen_clear(struct screen *screen)
{
    memset(screen->pixels, 0, sizeof(screen->pixels));
}

bool screen_draw_sprite(struct screen *screen, int x, int y, const char *sprite, int num)
{
    bool pixel_collison = false;

    for (int ly = 0; ly < num; ly++)
    {

        char c = sprite[ly];
        for (int lx = 0; lx < 8; lx++)
        {
            // Check for collision
            if ((c & (0x80 >> lx)) == 0)
                continue;

            if (screen->pixels[(ly + y) % CHIP8_DISPLAY_HEIGHT][(lx + x) % CHIP8_DISPLAY_WIDTH])
            {
                pixel_collison = true;
            }

            screen->pixels[(ly + y) % CHIP8_DISPLAY_HEIGHT][(lx + x) % CHIP8_DISPLAY_WIDTH] ^= true;
        }
    }
    return pixel_collison;
}