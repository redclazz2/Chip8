#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include "chip8/chip8.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <random>

using namespace std;

Chip8::Chip8(Display *dis, Input *inp) : display(dis), input(inp), randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    std::copy(std::begin(font), std::end(font), memory + 0x050);
    PC = 0x200;
}

bool Chip8::Init()
{
    if (!Chip8::LoadROM("../roms/ProgEmulatorLogo.ch8"))
    {
        return false;
    }else{
        cout << "ROM Loaded." << endl;
    }

    if (!display->init())
    {
        return false;
    }else{
        cout << "Display Initialized." << endl;
    }

    return true;
}

bool Chip8::LoadROM(string rom)
{
    try
    {
        ifstream file(rom, ios::binary);

        if (file.is_open())
        {
            file.seekg(SEEK_END);
            streampos size = file.tellg();
            file.seekg(0);

            char *buffer = new char[size];
            file.read(buffer, size);
            file.close();

            for (long i = 0; i < size; i++)
            {
                memory[0x200 + i] = buffer[i];
            }

            delete[] buffer;

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

void Chip8::Update()
{
    while (running)
    {
        map<string, bool> keyboard = input->getKeyboardStatus();

        if (keyboard["EXIT"])
        {
            running = false;
            display->destroy();
        }

        if (keyboard["1"])
        {
            cout << "1" << endl;
        }

        if (keyboard["2"])
        {
            cout << "2" << endl;
        }

        if (keyboard["3"])
        {
            cout << "3" << endl;
        }

        if (keyboard["Q"])
        {
            cout << "Q" << endl;
        }

        if (keyboard["W"])
        {
            cout << "W" << endl;
        }

        if (keyboard["E"])
        {
            cout << "E" << endl;
        }

        if (keyboard["A"])
        {
            cout << "A" << endl;
        }

        if (keyboard["S"])
        {
            cout << "S" << endl;
        }

        if (keyboard["D"])
        {
            cout << "D" << endl;
        }

        if (keyboard["Z"])
        {
            cout << "Z" << endl;
        }

        if (keyboard["X"])
        {
            cout << "X" << endl;
        }

        if (keyboard["C"])
        {
            cout << "C" << endl;
        }

        display->update();
    }
}