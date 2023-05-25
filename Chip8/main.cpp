#include <iostream>
#include <SDL.h>

//SDL Components
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
}sdl_t;

//Configuration
typedef struct {
    uint32_t window_height;
    uint32_t window_width;
    uint32_t scale_factor;
    uint32_t fg_color;
    uint32_t bg_color;
}config_t;

//Emulator Status
typedef enum {
    QUIT,
    RUNNING,
    PAUSED,
}emulator_state_t;

//Chip 8 Instructions
typedef struct {
    uint16_t opcode; 
    uint16_t NNN;   //12 Bit Pointer
    uint8_t NN;     //8 Bit Number
    uint8_t N;      //4 Bit Number
    uint8_t X;      //X Screen Coordinate
    uint8_t Y;      //Y Screen Coordinate
}instruction_t;

//Chip 8 Components
typedef struct {
    emulator_state_t state;
    uint8_t ram[4096];
    bool display[64 * 32];  //Original CHIP 8 Resolution
    uint16_t stack[12];     //Subroutine
    uint16_t* stack_ptr;    //Stack Pointer
    uint8_t V[16];          //Data Registers
    uint16_t I;             //I register
    uint8_t delay_timer;    //Program Delay at 60Hz, 0>
    uint8_t sound_timer;    //Play tones at 60Hz, 0>
    bool keypad[16];        //Hex Keyboard
    const char* rom_name;   //Currently Emulated ROM
    uint16_t PC;            //Program Counter
    instruction_t inst;     //Executable Instructions
}chip8_t;

/// <summary>
/// Initialize SDL related components and variables
/// </summary>
/// <param name="window"></param>
/// <param name="renderer"></param>
/// <returns></returns>
bool init_sdl(sdl_t *sdl,config_t *config){
    bool _return = true;
    
    #pragma region SDL Components Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        SDL_Log("An Error has ocurred with SDL. -INIT ERROR-\n");
        _return = false;
    }
    #pragma endregion

    #pragma region SDL Window Creation
    sdl->window = SDL_CreateWindow("Chip 8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        config->window_width * config->scale_factor, config->window_height * config->scale_factor, 0);

    if (!sdl->window) {
        SDL_Log("An Error has ocurred with SDL Window. -WINDOW ERROR-\n");
        _return = false;
    }
    #pragma endregion

    #pragma region SDL Renderer
    sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);

    if (!sdl->renderer) {
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
bool init_chip8(chip8_t* chip8,const char rom_name[]) {
    const uint32_t entry_point = 0x200; //Generally Chip 8 Programs Start at 0x200
    const uint8_t font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    //Copy Font to RAM
    memcpy(&chip8-> ram[0], font, sizeof(font));

    #pragma warning(disable:4996)
    FILE* rom = fopen(rom_name, "rb");
    if (!rom) {

        SDL_Log("Rom File is invalid or doesnt exists");
        return false;
    }

    fseek(rom, 0, SEEK_END);
    const size_t rom_size = ftell(rom);
    const size_t max_size = sizeof chip8->ram - entry_point;
    rewind(rom);

    if (rom_size > max_size) {
        SDL_Log("Rom Size is Too Big.");
        return false;
    }

    if (fread(&chip8->ram[entry_point], rom_size, 1, rom) != 1) {
        SDL_Log("Unable to load ROM data to Chip 8 RAM.");
        return false;
    }
    fclose(rom);
    
    //Set Chip 8 Status
    chip8->state = RUNNING;
    chip8->PC = entry_point;
    chip8->rom_name = rom_name;
    chip8->stack_ptr = &chip8->stack[0];
    return true;
}

/// <summary>
/// Clear program for finalization
/// </summary>
/// <param name=""></param>
void final_cleanup(sdl_t *sdl){
    SDL_DestroyWindow(sdl->window);
    SDL_DestroyRenderer(sdl->renderer);
    SDL_Quit();
}

/// <summary>
/// Clear the screen to background color
/// </summary>
/// <param name=""></param>
void clear_screen(const sdl_t *sdl,const config_t *config){
    const uint8_t r = config->bg_color >> 24 & 0xFF;
    const uint8_t g = config->bg_color >> 16 & 0xFF;
    const uint8_t b = config->bg_color >> 8 & 0xFF;
    const uint8_t a = config->bg_color >> 0 & 0xFF;

    SDL_SetRenderDrawColor(sdl->renderer, r, g, b, a);
    SDL_RenderClear(sdl->renderer);
}

/// <summary>
/// Calls SLD_RenderPresent to draw to screen
/// </summary>
/// <param name="renderer"></param>
void update_screen(sdl_t sdl,const chip8_t chip8,const config_t config) {
    SDL_Rect rect = {0,0,config.scale_factor,config.scale_factor};

    #pragma region Background Colors
    const uint8_t br = config.bg_color >> 24 & 0xFF;
    const uint8_t bg = config.bg_color >> 16 & 0xFF;
    const uint8_t bb = config.bg_color >> 8 & 0xFF;
    const uint8_t ba = config.bg_color >> 0 & 0xFF;
    #pragma endregion

    #pragma region Foreground Colors
    const uint8_t fr = config.fg_color >> 24 & 0xFF;
    const uint8_t fg = config.fg_color >> 16 & 0xFF;
    const uint8_t fb = config.fg_color >> 8 & 0xFF;
    const uint8_t fa = config.fg_color >> 0 & 0xFF;
    #pragma endregion


    //Loop through display pixels, draw a rectangle per pixel to the SDL Window
    for (uint32_t i = 0; i < sizeof chip8.display; i++) {
        rect.x = (i % config.window_width) * config.scale_factor;
        rect.y = (i / config.window_height) * config.scale_factor;

        if (chip8.display[i]) {
            //Pixel is on draw foreground
            SDL_SetRenderDrawColor(sdl.renderer,fr,fg,fb,fa);
            SDL_RenderFillRect(sdl.renderer,&rect);

            //Adds pixelated effect
            SDL_SetRenderDrawColor(sdl.renderer, br, bg, bb, ba);
            SDL_RenderDrawRect(sdl.renderer, &rect);
        }else {
            //Pixel is Off
            SDL_SetRenderDrawColor(sdl.renderer, br, bg, bb, ba);
            SDL_RenderFillRect(sdl.renderer, &rect);
        }
    }

    SDL_RenderPresent(sdl.renderer);
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

                    case SDLK_SPACE:
                        if (chip8->state == RUNNING){
                            chip8->state = PAUSED;
                            puts("----PAUSED----");
                        }
                        else {
                            chip8->state = RUNNING;
                        }
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

/// <summary>
/// Emulates Chip 8 Instructions
/// </summary>
/// <param name="chip8"></param>
void emulate_instruction(sdl_t *sdl,chip8_t *chip8, const config_t *config){
    //printf("Emulating Instruction\n");
    chip8->inst.opcode = (chip8->ram[chip8->PC] << 8) | chip8->ram[chip8->PC + 1];
    chip8->PC += 2;

    //Instruction Format
    chip8->inst.NNN = chip8->inst.opcode & 0x0FFF;
    chip8->inst.NN = chip8->inst.opcode & 0x0FF;
    chip8->inst.N = chip8->inst.opcode & 0x0F;
    chip8->inst.X = (chip8->inst.opcode >> 8) & 0x0F;
    chip8->inst.Y = (chip8->inst.opcode >> 4) & 0x0F;

    switch((chip8 -> inst.opcode >> 12) & 0x0F){

        case 0x0A:
            printf("R:Set I to NNN\n");
            chip8->I = chip8->inst.NNN;
            break;

        case 0x00:                
            if (chip8->inst.NN == 0xE0) { 
                //Clear Screen
                printf("R:Clear Screen\n");
                memset(&chip8->display[0], false, sizeof chip8->display);
            }else if (chip8->inst.NN == 0xEE){               
                //Grab last address from subroutine stack (pop!) so next opcode will be gotten from that address
                printf("R:Return from subroutine\n");
                chip8->PC = *--chip8->stack_ptr;
            }else {
                printf("E|0X00:Unimplemented Instruction\n");
            }
            break;

        case 0x01:
            printf("R:Jumping to new memory address\n");
            chip8->PC = chip8->inst.NNN;
            break;

        case 0x02:
            printf("R:Returned from subroutine\n");
            //Call Subroutine
            *chip8->stack_ptr = chip8->PC; //Store Current Address To return on subroutine stack
            chip8->PC = chip8->inst.NNN; //Program counter to subroutine address so that the next opcode isgotten from there
            break;

        case 0x06:
            printf("R:Set V to NN\n");
            chip8->V[chip8->inst.X] = chip8->inst.NN;
            break;

        case 0x07:
            printf("R:Set V to += NN\n");
            chip8->V[chip8->inst.X] += chip8->inst.NN;
            break;
            break;

        case 0x0D:
        {
            /*Draws a sprite to screen and sets VF to true, signaling a collision event
              To draw a sprite you start at the top left of the screen.*/

            printf("R:Draw to Display\n");

            uint8_t X_coord = chip8->V[chip8->inst.X] % config->window_width;
            uint8_t Y_coord = chip8->V[chip8->inst.Y] % config->window_height;
            const uint8_t orig_x = X_coord; //Original X Value

            chip8->V[0xF] = 0; //Initialize carry flag to 0

            //Loops all rows of sprites
            for (uint8_t i = 0; i < chip8->inst.N; i++) {
                const uint8_t sprite_data = chip8->ram[chip8->I + i];
                X_coord = orig_x; //Reset x for next row draw

                for (int8_t j = 7; j >= 0; j--) {
                    bool* pixel = &chip8->display[Y_coord * config->window_height + X_coord];
                    bool sprite_bit = (sprite_data & (1 << j));

                    //If sprite pixel is on and display pixel is on set carry flag
                    if (sprite_bit && *pixel) {
                        chip8->V[0xF] = 1;
                    }

                    // Exclusive OR to set pixel on or off
                    *pixel ^= sprite_bit;

                    if (++X_coord >= config->window_width) break; //Stop if X of sprite is greater than width
                }

                if (++Y_coord >= config->window_height) break; //Stop if Y of sprite is greater than height
            }
            break; 
        }

        default:
            printf("E:Unimplemented Instruction\n");
            break;
    }
}

int main(int argc, char* argv[]) {
    //Default
    if (argc < 2) {
        fprintf(stderr,"USSAGE: %s <rom_name>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    //Initialize Configuration
    config_t config = { 32,64,20,0xFFFFFFFF,0x00000000 };

    //Initialize SDL
    sdl_t sdl = { 0 };
    if (!init_sdl(&sdl,&config)) exit(EXIT_FAILURE);

    //Initialize Chip8 Machine
    chip8_t chip8 = {};
    const char* rom_name = argv[1];
    if (!init_chip8(&chip8,rom_name)) exit(EXIT_FAILURE);

    //Init Clear Screen
    clear_screen(&sdl,&config);

    //Main Loop
    while (chip8.state != QUIT){
        //Handle Input
        handle_input(&chip8);

        if (chip8.state == PAUSED) continue;
        //Get_Time();
        //Emulate Chip 8 Instructions
        emulate_instruction(&sdl,&chip8,&config);
        //Get_Time(); elapsed time since last function call

        //Delay for 16ms to render at 60hz
        //SDL_Delay(16 - actual time elapsed);
        SDL_Delay(16);
        update_screen(sdl,chip8,config);
    }

    //Clean up
    final_cleanup(&sdl);
    exit(EXIT_SUCCESS);
}