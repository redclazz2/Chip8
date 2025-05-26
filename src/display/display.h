#include<stdio.h>
#include<SDL3/SDL.h>

class display{
    public:
        int WIDTH = 800;
        int HEIGHT = 600;
        int DELAY = 3000;
        SDL_Window *window = NULL; 

        display();
    
        bool createWindow();
        void destroyWindow(); 
        void updateWindow();
};