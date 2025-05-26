#include <iostream>
#include "chip8/chip8.h"
#include "display/display.h"
#include <SDL3/SDL.h>

int main()
{
    display *dis = new display();
    chip8 *em = new chip8(dis);

    SDL_Event event;

    if (!em->init())
    {
        dis->destroyWindow();
        delete em;
        delete dis;
    }

    return 0;
}
