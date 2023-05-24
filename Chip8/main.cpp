#include <iostream>
#include <SDL.h>

const int WIDTH = 800, HEIGHT = 600;

bool init_sdl(void) {
    bool _return = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        SDL_Log("Failed to Init SDL systems: ", SDL_GetError());
        _return = false;
    }
    return _return;
}

void final_cleanup() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    //Initialize SDL
    if (!init_sdl()) exit(EXIT_FAILURE);

    final_cleanup();
    exit(EXIT_SUCCESS);
}