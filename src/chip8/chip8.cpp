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
#include <array>

using namespace std;

Chip8::Chip8(Display *dis, Input *inp) : display(dis), input(inp), randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    std::copy(std::begin(font), std::end(font), memory + 0x050);
    PC = 0x200;
}

bool Chip8::Init()
{
    if (!Chip8::LoadROM("../roms/tetris.rom"))
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
            file.seekg(0, std::ios::end);
            std::streampos size = file.tellg();
            file.seekg(0, std::ios::beg);

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

    map<string, bool> keyboard = input->getKeyboardStatus();
    array<bool, 16> chipKeyboard = input->getChipKeyboard(keyboard);

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

    case 3:
        if (V[X] == NN)
        {
            PC += 2;
        }
        break;

    case 4:
        if (V[X] != NN)
        {
            PC += 2;
        }
        break;

    case 5:
        if (V[X] == V[Y])
        {
            PC += 2;
        }
        break;

    case 6:
        V[X] = NN;
        break;

    case 7:
        V[X] += NN;
        break;

    // Logical and Arithmetic Instructions
    case 8:

        switch (N)
        {
        case 0:
            V[X] = V[Y];
            break;

        case 1:
            V[X] = V[X] | V[Y];
            break;

        case 2:
            V[X] = V[X] & V[Y];
            break;

        case 3:
            V[X] = V[X] ^ V[Y];
            break;

        case 4:
        {
            int sum = V[X] + V[Y];
            V[0xF] = sum > 0xFF ? 1 : 0;
            V[X] = sum;
            break;
        }

        case 5:
            V[0xF] = V[X] > V[Y] ? 1 : 0;
            V[X] = V[X] - V[Y];
            break;

        case 6:
            V[0xF] = V[X] & 0x01;
            V[X] = V[X] >> 1;
            break;

        case 7:
            V[0xF] = V[Y] > V[X] ? 1 : 0;
            V[X] = V[Y] - V[X];
            break;

        case 0xE:
            V[0xF] = (V[X] & 0x80) >> 7;
            V[X] = V[X] << 1;
            break;

        default:
            cout << "No suitable logic/arith opcode found: " << int(N) << endl;
            break;
        }

        break;

    case 0x9:
        if (V[X] != V[Y])
        {
            PC += 2;
        }
        break;

    case 0xA:
        I = NNN;
        break;

    case 0xB:
        PC = NNN + V[0];
        break;

    case 0xC:
        V[X] = NN & randByte(randGen);
        break;

    case 0xD:
    {
        uint8_t pX = V[X] % display->WIDTH;
        uint8_t pY = V[Y] % display->HEIGHT;

        V[0xF] = 0;

        for (int i = 0; i < N; i++) // Height
        {
            uint8_t sByte = memory[I + i];
            for (int e = 0; e < 8; e++) // Width
            {
                uint8_t sBit = sByte & (0x80 >> e);

                if (sBit)
                {
                    if (display->data[pX + e][pY + i])
                    {
                        display->data[pX + e][pY + i] = 0;
                        V[0xF] = 1;
                    }
                    else
                    {
                        display->data[pX + e][pY + i] ^= sBit;
                    }
                }
            }
        }
        break;
    }

    case 0xE:
        switch (N)
        {
        case 0x1:
            if (!chipKeyboard[V[X]])
                PC += 2;
            break;

        case 0xE:
            if (chipKeyboard[V[X]])
                PC += 2;
            break;

        default:
            cout << "No suitable 0xEXXN instruction found" << endl;
            break;
        }

        break;

    case 0xF:
        switch (NN)
        {
        case 0x1E:
            I += V[X];
            break;

        case 0x15:
            delayTimer = V[X];
            break;

        case 0x07:
            V[X] = delayTimer;
            break;

        case 0x18:
            soundTimer = V[X];
            break;

        case 0x0A:
        {
            bool keyPressed = false;

            for (int i = 0; i < 16; i++)
            {
                if (chipKeyboard[i])
                {
                    keyPressed = true;
                    V[X] = i;
                    break;
                }
            }

            if (!keyPressed)
            {
                PC -= 2;
            }
            break;
        }

        case 0x29:
            I = 0x50 + (V[X] * 5);
            break;

        case 0x33:
        {
            uint8_t value = V[X];
            for (int i = 2; i >= 0; i--)
            {
                memory[I + i] = value % 10;
                value /= 10;
            }
            break;
        }

        case 0x55:
            for (int i = 0; i <= X; i++)
            {
                memory[I + i] = V[i];
            }
            break;

        case 0x65:
            for (int i = 0; i <= X; i++)
            {
                V[i] = memory[I + i];
            }
            break;

        default:
            cout << "No suitable 0xFXXX opcode found: " << int(NN) << endl;
            break;
        }
        break;

    default:
        cout << "No suitable opcode found: " << int(firstNibble) << endl;
        break;
    }

    display->update();

    if (keyboard["EXIT"])
    {
        running = false;
        display->destroy();
    }
}

void Chip8::UpdateTimers()
{
    if (delayTimer > 0)
    {
        delayTimer -= 1;
    }

    if (soundTimer > 0)
    {
        soundTimer -= 1;
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