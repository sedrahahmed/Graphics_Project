
#define GLFW_INCLUDE_NONE   // we only need GLFW functions they create windows and tell us what keyboard and mouse are doing
#include <GLFW/glfw3.h>
#include "Input.h"
#include "Config.h"     

static GLFWwindow* inputWindow = nullptr;//pointer to window

static bool keyNow[GLFW_KEY_LAST + 1] = {};// One true/false per key(true law pressed) GLFW key codes go up to GLFW_KEY_LAST fa +1 bec array starts at 0
static bool keyBefore[GLFW_KEY_LAST + 1] = {};//same bas for prev frame

static bool leftNow = false, leftBefore = false;
static bool rightNow = false, rightBefore = false;//lel mouse

static glm::vec2 mouse(0.0f, 0.0f);//mouse x mouse y

void initInput(GLFWwindow* window) {
    inputWindow = window;
}

void updateInput() {//runs once every frame
    if (!inputWindow) return;


    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {//loop over all keys from 32 to end
        keyBefore[key] = keyNow[key];
        keyNow[key] = glfwGetKey(inputWindow, key) == GLFW_PRESS;// asks if pressed or no
    }

    // same for mouse
    leftBefore = leftNow;
    rightBefore = rightNow;
    leftNow = glfwGetMouseButton(inputWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    rightNow = glfwGetMouseButton(inputWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
     
    double cx, cy;
    glfwGetCursorPos(inputWindow, &cx, &cy);//px where cursos is
    int winW, winH;
    glfwGetWindowSize(inputWindow, &winW, &winH);//px of window w and h
    if (winW > 0 && winH > 0) {
        mouse.x = (float)cx * SCREEN_W / winW;//convert from window px to the game px scale from window size to game size in case the window was resized
        mouse.y = SCREEN_H - (float)cy * SCREEN_H / winH;//ben flip bec +ve ta7t our game origin bottom left glfw 00 top left
    }
}

glm::vec2 mousePos() { return mouse; }
bool mouseClicked() { return leftNow && !leftBefore; }
bool mouseRightClicked() { return rightNow && !rightBefore; }//ignore law holding value

bool keyPressed(int key) {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return keyNow[key] && !keyBefore[key];
}//went down nowin this frame?

bool keyHeld(int key) {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return keyNow[key];//is it down
}

bool mouseInRect(float x, float y, float w, float h) {//centre then width height
    glm::vec2 m = mousePos();
    return m.x >= x - w / 2 && m.x <= x + w / 2 && m.y >= y - h / 2 && m.y <= y + h / 2;
}


// The game loop in main checks glfwWindowShouldClose and stops if yes
void requestExit() {
    if (inputWindow) glfwSetWindowShouldClose(inputWindow, true);
}
