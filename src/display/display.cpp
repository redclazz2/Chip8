#include "display.h"
#include <iostream>

using namespace std;

Display::Display()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Failed to initialize SDL";
    }
}

bool Display::createWindow()
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
    }

    return created;
}

void Display::destroyWindow()
{
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}