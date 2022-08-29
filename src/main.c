#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "chip8.h"

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6,
    SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d,
    SDLK_e, SDLK_f
};

int main(int argc, char const *argv[])
{

    struct chip8 chip8;
    chip8_init(&chip8);

    chip8_screen_draw_sprite(&chip8.screen, 0, 0, &chip8.memory.memory[0x00], 5);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
            EMULATOR_WINDOW_TITLE, 
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            CHIP8_WINDOW_WIDTH * CHIP8_WINDOW_SCALE,
            CHIP8_WINDOW_HEIGHT * CHIP8_WINDOW_SCALE,
            SDL_WINDOW_MAXIMIZED
    );
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

SDL_Event e;

bool quit = false;
while (!quit){
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
            quit = true;
        }
        if (e.type == SDL_KEYDOWN){
            int vkey = chip8_keyboard_map(keyboard_map, e.key.keysym.sym);
            if (vkey != -1)
            {
                chip8_keyboard_down(&chip8.keyboard, vkey);
            }
        }
        if (e.type == SDL_KEYUP){
            int vkey = chip8_keyboard_map(keyboard_map, e.key.keysym.sym);
            if (vkey != -1)
            {
                chip8_keyboard_up(&chip8.keyboard, vkey);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

    for (int x = 0; x < CHIP8_WINDOW_WIDTH; x++)
    {
        for (int y = 0; y < CHIP8_WINDOW_HEIGHT; y++)
        {
            if (chip8_screen_is_set(&chip8.screen, x, y))
            {
                //printf("%i ", x);
                //printf("%i \n", y);
                SDL_Rect rectangle;
                rectangle.x = x * CHIP8_WINDOW_SCALE;
                rectangle.y = y * CHIP8_WINDOW_SCALE;
                rectangle.h = CHIP8_WINDOW_SCALE;
                rectangle.w = CHIP8_WINDOW_SCALE;
                SDL_RenderFillRect(renderer, &rectangle);
            }
        }
    }
    SDL_RenderPresent(renderer);
}
    SDL_DestroyWindow(window);
    return 0;
}
