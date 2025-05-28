#include "display.h"
#include <iostream>
#include <SDL3/SDL.h>

using namespace std;

Display::Display()
{
    data.resize(WIDTH * HEIGHT * UPSCALE * UPSCALE);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Failed to initialize SDL";
    }
}

bool Display::createWindow()
{
    bool created = true;

    // Create a window
    window = SDL_CreateWindow(
        "Red's Chip 8",
        WIDTH,
        HEIGHT,
        0);

    if (window == NULL)
    {
        fprintf(stderr, "Window failed to initialize %s\n", SDL_GetError());
        created = false;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(
        window, NULL);

    if (renderer == NULL)
    {
        fprintf(stderr, "Unable to create renderer %s\n", SDL_GetError());
        created = false;
    }

    // Create a texture
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC, //Suitable for this use case
        WIDTH,
        HEIGHT);

    if (texture == NULL)
    {
        fprintf(stderr, "Unable to create texture %s\n", SDL_GetError());
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

    if(renderer != NULL){
        SDL_DestroyRenderer(renderer);
    }

    if(texture != NULL){
        SDL_DestroyTexture(texture);
    }

    SDL_Quit();
}

void Display::updateWindow()
{
   SDL_UpdateTexture(texture, NULL, data.data(), WIDTH/8 * sizeof(uint8_t));
   SDL_RenderClear(renderer);
   SDL_RenderTexture(renderer, texture, NULL, NULL);
   SDL_RenderPresent(renderer);
   SDL_Delay(DELAY);
}