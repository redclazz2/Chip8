#pragma once
#include<SDL3/SDL.h>

class Display{
    public:
        int WIDTH = 800;
        int HEIGHT = 600;
        int DELAY = 3000;
        SDL_Window *window = NULL; 

        Display();
    
        bool createWindow();
        void destroyWindow(); 
        void updateWindow();
};