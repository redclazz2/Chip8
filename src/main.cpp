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
        const int CPU_HZ = 500;
        const int TIMER_HZ = 60;

        const Uint32 CPU_INTERVAL = 1000 / CPU_HZ;
        const Uint32 TIMER_INTERVAL = 1000 / TIMER_HZ;

        Uint32 lastCpuTick = SDL_GetTicks();
        Uint32 lastTimerTick = SDL_GetTicks();

        while (em->running)
        {
            Uint32 now = SDL_GetTicks();

            if (now - lastCpuTick >= CPU_INTERVAL)
            {
                em->Update(); 
                lastCpuTick = now;
            }

            if (now - lastTimerTick >= TIMER_INTERVAL)
            {
                em->UpdateTimers();
                lastTimerTick = now;
            }

            //SDL_Delay(1);
        }
    }
    
    delete em;
    delete dis;
    delete in;

    SDL_Quit();
    return 0;
}
