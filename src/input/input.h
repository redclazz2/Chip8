#pragma once
#include<stdio.h>
#include<map>
#include<string>
#include<SDL3/SDL.h>

class Input{
    private:
        std::map<std::string, bool>  keyboard;
    public:
        std::map<std::string,bool> getKeyboardStatus();
};