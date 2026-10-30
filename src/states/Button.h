#pragma once
// Button.h: a reusable menu button.
// Owner: Person A

#include <string>

struct Button {
    float x, y;          // center
    float w, h;
    std::string label;
};

bool isHovered(const Button& b);   // mouse is over it
bool isClicked(const Button& b);   // clicked THIS frame
void drawButton(const Button& b);  // box + label, lighter when hovered
