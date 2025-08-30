#include <iostream>
#include "chip8/chip8.h"
#include <display/display.h>

int main()
{
    Display *dis = new Display();
    Input *in = new Input();
    Chip8 *em = new Chip8(dis, in);

    if (em->Init())
    {
        em->Update();
    }
    
    delete em;
    delete dis;
    delete in;

    SDL_Quit();

    return 0;
}
