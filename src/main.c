#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6,
    SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d,
    SDLK_e, SDLK_f
};

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("You must provide a file to lead.");
        return -1;
    }

    const char* filename = argv[1];
    printf("Filename to read is %s\n", filename);

    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        printf("failed to open file");
        return -1;
    }
    
    fseek(file, 0, SEEK_END); //go to end of file
    long size = ftell(file); // get current position = file size
    fseek(file, 0, SEEK_SET); //set pointer to begining of file

    char buffer[size];
    int result = fread(buffer, size, 1, file);
    if (result != 1)
    {
        printf("failed to lead file");
        return -1;
    }

    struct chip8 chip8;
    chip8_init(&chip8);
    chip8_load(&chip8, buffer, size);
    chip8_keyboard_set_map(&chip8.keyboard, keyboard_map);



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

    if (chip8.registers.delay_timer > 0)
    {
        printf("sleep \n");
        int delay = 100;
        //sleep:
        #ifdef _WIN32
        Sleep(pollingDelay);
        #else
        usleep(delay * 1000);  /* sleep for 100 milliSeconds */
        #endif
        chip8.registers.delay_timer -= 1;
    }

    // TODO: implementar mejor sonido
    if (chip8.registers.sound_timer > 0){
        //printf("\a");
        chip8.registers.sound_timer -= 1;
    }

    unsigned short opcode = chip8_memory_get_short(&chip8.memory, chip8.registers.program_counter);
    chip8.registers.program_counter += 2;
    chip8_exec(&chip8, opcode);
}
    SDL_DestroyWindow(window);
    return 0;
}
