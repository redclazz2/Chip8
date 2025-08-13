#include "display.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <cstdlib>
#include <ctime>

using namespace std;

Display::Display()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Failed to initialize SDL";
    }
}

bool Display::init()
{
    bool created = true;

    window = SDL_CreateWindow(
        "Red's Chip 8",
        WIDTH * UPSCALE,
        HEIGHT * UPSCALE,
        0);

    if (window == NULL)
    {
        fprintf(stderr, "Window failed to initialize %s\n", SDL_GetError());
        created = false;
    }

    renderer = SDL_CreateRenderer(
        window, NULL);

    if (renderer == NULL)
    {
        fprintf(stderr, "Unable to create renderer %s\n", SDL_GetError());
        created = false;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        WIDTH,
        HEIGHT);

    if (texture == NULL)
    {
        fprintf(stderr, "Unable to create texture %s\n", SDL_GetError());
        created = false;
    }

    return created;
}

void Display::destroy()
{
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }

    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }

    SDL_Quit();
}

void Display::update()
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    /*for (int n = 0; n < 5; ++n)
    {
        int rx = rand() % WIDTH;
        int ry = rand() % HEIGHT;
        data[rx][ry] = true;
    }*/

    for (int i = 0; i < WIDTH; i++)
    {
        for (int e = 0; e < HEIGHT; e++)
        {
            if (data[i][e])
            {
                SDL_FRect rect;
                rect.x = i * UPSCALE;
                rect.y = e * UPSCALE;

                rect.w = UPSCALE;
                rect.h = UPSCALE;

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_Delay(DELAY);
}