#pragma once
#include<SDL3/SDL.h>
#include <vector>

class Display{
    public:
        int UPSCALE  = 10;
        int WIDTH = 64 * UPSCALE;
        int HEIGHT = 32 * UPSCALE;
        int DELAY = 100;
        
        std::vector<uint8_t> data;
        
        SDL_Window *window = NULL; 
        SDL_Renderer *renderer = NULL;
        SDL_Texture *texture = NULL;

        Display();
    
        bool createWindow();
        void destroyWindow(); 
        void updateWindow();
};