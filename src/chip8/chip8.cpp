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
#include <thread>
#include <unistd.h>

using namespace std;

Chip8::Chip8(Display *dis, Input *inp) : display(dis), input(inp), randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    std::copy(std::begin(font), std::end(font), memory + 0x050);
    PC = 0x200;
}

bool Chip8::Init()
{
    if (!Chip8::LoadROM("../roms/IBMLogo.ch8"))
    {
        return false;
    }
    else
    {
        cout << "ROM Loaded." << endl;
        DumpMemoryFromProgramStart();
    }

    if (!display->init())
    {
        return false;
    }
    else
    {
        memset(display->data, 1, sizeof(display->data));
        cout << "Display Initialized." << endl;
        display->update();
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
            file.seekg(std::ios::end);
            streamsize size = file.tellg();
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

        uint16_t instruction = (memory[PC] << 8u) | memory[PC + 1];
        PC += 2;

        //  ---1---   ---2---  -> In bytes
        // 1010 0000 0000 0000 -> Writen in binary
        //  -1-  -2-  -3-  -4-   -> Nibbles
        uint8_t firstNibble = (instruction & 0xF000) >> 12;
        uint8_t X = (instruction & 0x0F00) >> 8;
        uint8_t Y = (instruction & 0x00F0) >> 4;
        uint8_t N = (instruction & 0x000F);

        uint8_t NN = (instruction & 0x00FF);
        uint16_t NNN = (instruction & 0x0FFF);

        switch (firstNibble)
        {
        case 0:
            switch (instruction)
            {
            case 0x00E0:
                memset(display->data, 0, sizeof(display->data));
                break;

            case 0x00EE:
                SP--;
                PC = stack[SP];
                break;
            }
            break;

        case 1:
            PC = NNN;
            break;

        case 2:  
            stack[SP] = PC;
            SP++;    

            PC = NNN;
            break;

        case 6:
            V[X] = NN;
            break;

        case 7:
            V[X] += NN;
            break;

        case 0xA:
            I = NNN;
            break;

        case 0xD:
            uint8_t pX = V[X] % display->WIDTH;
            uint8_t pY = V[Y] % display->HEIGHT;
            
            V[0xF] = 0;

            for(int i = 0; i < N; i++){
                uint8_t spriteData = memory[I];
            }

            break;
        }

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

        sleep(2);
    }
}

void Chip8::DumpMemoryFromProgramStart()
{
    constexpr std::size_t PROGRAM_START = 0x200;
    constexpr std::size_t MEMORY_SIZE = sizeof(memory);

    for (std::size_t addr = PROGRAM_START; addr < MEMORY_SIZE; addr += 16)
    {
        std::cout << std::hex << std::setw(4) << std::setfill('0') << addr << ": ";

        for (std::size_t i = 0; i < 16 && (addr + i) < MEMORY_SIZE; i++)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(memory[addr + i]) << " ";
        }

        std::cout << "\n";
    }

    std::cout << std::dec; // back to decimal
}