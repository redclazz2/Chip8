#include <input/input.h>
#include <map>
#include <string>
#include <SDL3/SDL.h>
#include <iostream>

using namespace std;

map<string, bool> Input::getKeyboardStatus()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            keyboard["EXIT"] = true;
            break;

        case SDL_EVENT_KEY_DOWN:
            keyboard[SDL_GetKeyName(event.key.key)] = true;
            break;

        case SDL_EVENT_KEY_UP:
            keyboard[SDL_GetKeyName(event.key.key)] = false;
            break;
        }
    }

    return keyboard;
}