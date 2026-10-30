// main.cpp: opens the window and runs the game loop. Owner: Person A
#include <glad/glad.h>        // must be BEFORE GLFW
#include <GLFW/glfw3.h>
#include <iostream>

#include "Config.h"
#include "Draw.h"
#include "Input.h"
#include "StateManager.h"

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   // required on Mac
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Carnival of the Cursed", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load GLAD" << std::endl;
        return -1;
    }

    int fbW, fbH;                                  // Mac Retina fix
    glfwGetFramebufferSize(window, &fbW, &fbH);
    glViewport(0, 0, fbW, fbH);

    glEnable(GL_BLEND);                            // transparency (sprites, glows, highlights)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initDraw();
    initInput(window);
    // TODO later: initText(ASSET_DIR "fonts/...", 48); initSound();

    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;

        updateInput();
        updateCurrentState(dt);

        glClearColor(0.15f, 0.05f, 0.2f, 1.0f);   // dark carnival purple
        glClear(GL_COLOR_BUFFER_BIT);
        drawCurrentState();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
