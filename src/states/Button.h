#pragma once

#include <string>

struct Button {
    float x, y;       
    float w, h;
    std::string label;
};

bool isHovered(const Button& b);   
bool isClicked(const Button& b);   
void drawButton(const Button& b); 
