#pragma once
// Input.h: mouse and keyboard, in the same coordinates as the screen (origin bottom-left).
// Owner: Person A

#include <glm/glm.hpp>

struct GLFWwindow;   // forward declaration: avoids including GLFW in every file

void initInput(GLFWwindow* window);   // call once after creating the window
void updateInput();                   // call once at the START of every frame

glm::vec2 mousePos();          // already flipped: y = 0 at the bottom
bool mouseClicked();           // left button went down THIS frame
bool mouseRightClicked();      // right button went down THIS frame (cancel card selection)

// key = GLFW key code, e.g. GLFW_KEY_W, GLFW_KEY_ESCAPE
bool keyPressed(int key);      // went down THIS frame (one-time actions)
bool keyHeld(int key);         // is down right now

// Helper: is the mouse inside a rectangle? (x, y = center)
bool mouseInRect(float x, float y, float w, float h);
