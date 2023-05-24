#include <iostream>
#include <SDL.h>

SDL_Window *window;
SDL_Renderer* renderer;
uint32_t window_height = 32;
uint32_t window_width = 64;
uint32_t scale_factor = 20;
uint32_t fg_color = 0xFFFFFFFF;
uint32_t bg_color = 0x00000000;

//Emulator Status
typedef enum {
    QUIT,
    RUNNING,
    PAUSED,
}emulator_state_t;

//Chip 8 Status
typedef struct {
    emulator_state_t state;
}chip8_t;

/// <summary>
/// Initialize SDL related components and variables
/// </summary>
/// <param name="window"></param>
/// <param name="renderer"></param>
/// <returns></returns>
bool init_sdl(SDL_Window *window,SDL_Renderer *renderer){
    bool _return = true;
    
    #pragma region SDL Components Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        SDL_Log("An Error has ocurred with SDL. -INIT ERROR-\n");
        _return = false;
    }
    #pragma endregion

    #pragma region SDL Window Creation
    window = SDL_CreateWindow("Chip 8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width * scale_factor, window_height * scale_factor, 0);

    if (!window) {
        SDL_Log("An Error has ocurred with SDL Window. -WINDOW ERROR-\n");
        _return = false;
    }
    #pragma endregion

    #pragma region SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_Log("An Error has ocurred with SDL Renderer. -RENDERER ERROR-\n");
        _return = false;
    }
    #pragma endregion

    return _return;
}

/// <summary>
/// Initializes Chip 8 Status Machine
/// </summary>
/// <param name="chip8"></param>
/// <returns></returns>
bool init_chip8(chip8_t* chip8) {
    chip8->state = RUNNING;
    return true;
}

/// <summary>
/// Clear program for finalization
/// </summary>
/// <param name=""></param>
void final_cleanup(void){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

/// <summary>
/// Clear the screen to background color
/// </summary>
/// <param name=""></param>
void clear_screen(void){
    const uint8_t r = bg_color >> 24 & 0xFF;
    const uint8_t g = bg_color >> 16 & 0xFF;
    const uint8_t b = bg_color >> 8 & 0xFF;
    const uint8_t a = bg_color >> 0 & 0xFF;

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

/// <summary>
/// Calls SLD_RenderPresent to draw to screen
/// </summary>
/// <param name="renderer"></param>
void update_screen(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
}

/// <summary>
/// Handle user input with SDL input
/// </summary>
void handle_input(chip8_t *chip8) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                chip8->state = QUIT;
                break;

            case SDL_KEYDOWN:

                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        //Exit Window and End Program
                        chip8->state = QUIT;
                    break;
                }

                break;

            case SDL_KEYUP:
                break;

            default:
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    //Initialize SDL
    if (!init_sdl(window,renderer)) exit(EXIT_FAILURE);

    //Initialize Chip8 Machine
    chip8_t chip8 = {};
    if (!init_chip8(&chip8)) exit(EXIT_FAILURE);

    //Init Clear Screen
    clear_screen();

    //Main Loop
    while (chip8.state != QUIT){
        //Handle Input
        handle_input(&chip8);

        //Get_Time();
        //Emulate Chip 8 Instructions
        //Get_Time(); elapsed time since last function call

        //Delay for 16ms to render at 60hz
        //SDL_Delay(16 - actual time elapsed);
        SDL_Delay(16);
        update_screen(renderer);
    }

    //Clean up
    final_cleanup();
    exit(EXIT_SUCCESS);
}