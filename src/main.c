
#include <stdio.h>
#include <unistd.h>

#include "SDL2/SDL.h"

#include "chip8.h"
#include "config.h"
#include "keyboard.h"
#include "screen.h"

const char sdl_keyboard_map[] = {
	SDLK_0, SDLK_1, SDLK_2, SDLK_3,
	SDLK_4, SDLK_5, SDLK_6, SDLK_7,
	SDLK_8, SDLK_9, SDLK_a, SDLK_b,
	SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int main(int argc, char **argv)
{
	// Create and init the chip8 machine
	struct chip8 chip8;
	chip8_init(&chip8);
	keyboard_map_set(&chip8.keyboard, sdl_keyboard_map);

	// Load the rom
	if (argc != 2) {
		printf("Please specify a rom as an arg.");
		return -1;
	}

	const char *filename = argv[1];
	FILE *f = fopen(filename, "rb");

	if (!f)
	{
		printf("Failed to open file");
		return -1;
	}

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	int8_t buf[size];
	int res = fread(buf, size, 1, f);
	fclose(f);

	if (res != 1)
	{
		printf("Failed to read file.");
		return -1;
	}

	// Load the data into memory
	chip8_load(&chip8, buf, size);

	// Init SDL2
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE,
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  CHIP8_SCALED_DISPLAY_WITDH,
										  CHIP8_SCALED_DISPLAY_HEIGHT,
										  SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window,
												-1,
												SDL_TEXTUREACCESS_TARGET);

	while (1)
	{

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				goto out;
			break;

			case SDL_KEYDOWN:
			{
				uint8_t key = event.key.keysym.sym;
				int vkey = keyboard_map(chip8.keyboard.keyboard_map, key);
				if (vkey != -1)
				{
					keyboard_key_down(&chip8.keyboard, vkey);
				}
			}
			break;
			
			case SDL_KEYUP:
			{
				uint8_t key = event.key.keysym.sym;
				int vkey = keyboard_map(chip8.keyboard.keyboard_map, key);
				if (vkey != -1)
				{
					keyboard_key_up(&chip8.keyboard, vkey);
				}
			}
			break;
			}
		}

		// Lets draw some pixels
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

		for (int x = 0; x < CHIP8_DISPLAY_WIDTH; x++)
		{
			for (int y = 0; y < CHIP8_DISPLAY_HEIGHT; y++)
			{
				if (screen_is_set(&chip8.screen, x, y))
				{
					SDL_Rect rect;
					rect.h = CHIP8_DISPLAY_SCALE_FACTOR;
					rect.w = CHIP8_DISPLAY_SCALE_FACTOR;
					rect.x = x * CHIP8_DISPLAY_SCALE_FACTOR;
					rect.y = y * CHIP8_DISPLAY_SCALE_FACTOR;
					SDL_RenderFillRect(renderer, &rect);
				}
			}
		}

		SDL_RenderPresent(renderer);

		// Check for sleep timer
		if (chip8.registers.delay_timer > 0)
		{
			// Try to appox 60HZ ???
			usleep(50);
			chip8.registers.delay_timer -= 1;
		}

		if (chip8.registers.sound_timer > 0)
		{
			printf("Beep Emulation!!\n");
			usleep(50);
			chip8.registers.sound_timer = 0;
		}

		uint16_t opcode = memory_fetch_2bytes(&chip8.memory,
											  chip8.registers.program_counter);
		chip8.registers.program_counter += 2;
		chip8_exec(&chip8, opcode);
	}

out:
	SDL_DestroyWindow(window);
	return 0;
}