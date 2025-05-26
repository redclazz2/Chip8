#include "chip8.h"
#include "display.h"
#include <algorithm>
#include <iterator>

bool chip8::init(){
    if (!display->createWindow()) {
        return false;
    }

    return true;
}

void chip8::update(){
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }
        SDL_Delay(10);
    }
}