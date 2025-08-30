#pragma once
#include<SDL3/SDL.h>
#include <vector>

class Display{
    public:
        int UPSCALE  = 10;
        int WIDTH = 64;
        int HEIGHT = 32;
        int DELAY = 100;
        
        uint8_t data[64][32];
        
        SDL_Window *window = NULL; 
        SDL_Renderer *renderer = NULL;

        Display();
    
        bool init();
        void destroy(); 
        void update();
};