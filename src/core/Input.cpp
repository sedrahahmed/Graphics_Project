// Input.cpp. Owner: Person A
// Same idea as learnopengl's processInput(): ask GLFW every frame with glfwGetKey / glfwGetMouseButton.
// To know if something was pressed THIS frame, we also remember what it was LAST frame:
//   pressed this frame = down now AND was up last frame
#define GLFW_INCLUDE_NONE   // we only need GLFW's functions, not its OpenGL header
#include <GLFW/glfw3.h>
#include "Input.h"
#include "Config.h"         // SCREEN_W, SCREEN_H

static GLFWwindow* inputWindow = nullptr;

// One true/false per key. GLFW key codes go up to GLFW_KEY_LAST.
static bool keyNow[GLFW_KEY_LAST + 1] = {};
static bool keyBefore[GLFW_KEY_LAST + 1] = {};

static bool leftNow = false, leftBefore = false;
static bool rightNow = false, rightBefore = false;

static glm::vec2 mouse(0.0f, 0.0f);

void initInput(GLFWwindow* window) {
    inputWindow = window;
}

void updateInput() {
    if (!inputWindow) return;

    // Keys: valid GLFW key codes start at GLFW_KEY_SPACE (32)
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
        keyBefore[key] = keyNow[key];
        keyNow[key] = glfwGetKey(inputWindow, key) == GLFW_PRESS;
    }

    // Mouse buttons
    leftBefore = leftNow;
    rightBefore = rightNow;
    leftNow = glfwGetMouseButton(inputWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    rightNow = glfwGetMouseButton(inputWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    // Mouse position: GLFW gives it with (0,0) at the TOP-left of the window.
    // Our game has (0,0) at the BOTTOM-left, so flip y.
    // Also scale from window size to game size, in case the window was resized.
    double cx, cy;
    glfwGetCursorPos(inputWindow, &cx, &cy);
    int winW, winH;
    glfwGetWindowSize(inputWindow, &winW, &winH);
    if (winW > 0 && winH > 0) {
        mouse.x = (float)cx * SCREEN_W / winW;
        mouse.y = SCREEN_H - (float)cy * SCREEN_H / winH;
    }
}

glm::vec2 mousePos() { return mouse; }
bool mouseClicked() { return leftNow && !leftBefore; }
bool mouseRightClicked() { return rightNow && !rightBefore; }

bool keyPressed(int key) {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return keyNow[key] && !keyBefore[key];
}

bool keyHeld(int key) {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return keyNow[key];
}

bool mouseInRect(float x, float y, float w, float h) {
    glm::vec2 m = mousePos();
    return m.x >= x - w / 2 && m.x <= x + w / 2 && m.y >= y - h / 2 && m.y <= y + h / 2;
}
