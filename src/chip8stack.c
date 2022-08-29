#include "chip8.h"
#include "chip8stack.h"
#include <assert.h>

static void chip8_stack_in_bounds(struct chip8* chip8){
    assert(chip8 -> registers.stack_pointer < CHIP8_STACK_DEPTH);
}

void chip8_stack_push(struct chip8* chip8, unsigned short val){
    chip8_stack_in_bounds(chip8);
    chip8 -> stack.stack[chip8 -> registers.stack_pointer] = val;
    chip8 -> registers.stack_pointer += 1;
}

unsigned short chip8_stack_pop(struct chip8* chip8){
    chip8 -> registers.stack_pointer -= 1;
    chip8_stack_in_bounds(chip8);
    return chip8 -> stack.stack[chip8 -> registers.stack_pointer];
}