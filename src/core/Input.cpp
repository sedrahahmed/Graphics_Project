// Input.cpp. Owner: Person A
#include "Input.h"

void initInput(GLFWwindow* window) { }
void updateInput() { }

glm::vec2 mousePos() { return glm::vec2(0.0f, 0.0f); }
bool mouseClicked() { return false; }
bool mouseRightClicked() { return false; }

bool keyPressed(int key) { return false; }
bool keyHeld(int key) { return false; }

bool mouseInRect(float x, float y, float w, float h) {
    glm::vec2 m = mousePos();
    return m.x >= x - w / 2 && m.x <= x + w / 2 && m.y >= y - h / 2 && m.y <= y + h / 2;
}
