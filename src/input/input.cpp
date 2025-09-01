#include <input/input.h>
#include <map>
#include <string>
#include <SDL3/SDL.h>
#include <iostream>
#include <array>

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

array<bool,16> Input::getChipKeyboard(std::map<string,bool> sdlInput){
    std::array<bool, 16> chipKeyboard{};

    chipKeyboard[0x0] = sdlInput["1"];
    chipKeyboard[0x1] = sdlInput["2"];
    chipKeyboard[0x2] = sdlInput["3"];
    chipKeyboard[0x3] = sdlInput["4"];
    chipKeyboard[0x4] = sdlInput["Q"];
    chipKeyboard[0x5] = sdlInput["W"];
    chipKeyboard[0x6] = sdlInput["E"];
    chipKeyboard[0x7] = sdlInput["R"];
    chipKeyboard[0x8] = sdlInput["A"];
    chipKeyboard[0x9] = sdlInput["S"];
    chipKeyboard[0xA] = sdlInput["D"];
    chipKeyboard[0xB] = sdlInput["F"];
    chipKeyboard[0xC] = sdlInput["Z"];
    chipKeyboard[0xD] = sdlInput["X"];
    chipKeyboard[0xE] = sdlInput["C"];
    chipKeyboard[0xF] = sdlInput["V"];

    return chipKeyboard;
}