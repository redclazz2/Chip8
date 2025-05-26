#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include "chip8/chip8.h"
#include <iostream>

using namespace std;

Chip8::Chip8(Display *dis, Input* inp): display(dis), input(inp)
{
    // Copy the font to memory
    std::copy(std::begin(font), std::end(font), memory + 0x050);
}

bool Chip8::Init()
{
    if (!display->createWindow())
    {
        return false;
    }

    return true;
}

void Chip8::Update()
{
    while (running)
    {
        map<string,bool> keyboard = input->getKeyboardStatus();

        if(keyboard["EXIT"]){
            running = false;
            display->destroyWindow();
        }

        if(keyboard["Q"]){
            cout << "Q" << endl;
        }

        if(keyboard["W"]){
            cout << "W" << endl;
        }

        if(keyboard["E"]){
            cout << "E" << endl;
        }

        if(keyboard["A"]){
            cout << "A" << endl;
        }

        if(keyboard["S"]){
            cout << "S" << endl;
        }

        if(keyboard["D"]){
            cout << "D" << endl;
        }

        if(keyboard["Z"]){
            cout << "Z" << endl;
        }

        if(keyboard["X"]){
            cout << "X" << endl;
        }

        if(keyboard["C"]){
            cout << "C" << endl;
        }

        //SDL_Delay(10);
    }
}