#include "display.h"
#include <iostream>

using namespace std;

display::display()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Failed to initialize SDL";
    }
}

bool display::createWindow()
{
    bool created = true;

    window = SDL_CreateWindow(
        "Red's Chip 8",
        WIDTH,
        HEIGHT,
        0
    );

    if (window == NULL)
    {
        fprintf(stderr, "Window failed to initialize", SDL_GetError());
        created = false;
    }else{
        cout << "Created the SDL window";
    }

    return created;
}

void display::destroyWindow()
{
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}