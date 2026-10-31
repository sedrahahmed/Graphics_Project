// main.cpp: opens the window and runs the game loop. Owner: Person A
#include <glad/glad.h>        // must be BEFORE GLFW
#include <GLFW/glfw3.h>
#include <iostream>

#include "Config.h"
#include "Draw.h"
#include "Input.h"
#include "StateManager.h"
#include "Button.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
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
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

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
    unsigned int testTex = loadTexture(ASSET_DIR "characters/test_sprite.png");
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

        // ---- TEMP TEST (delete later) ----
        // All values are in pixels now: (0,0) = bottom-left, (1280,720) = top-right
        // drawRect(640, 360, 256, 72, 45.0f, glm::vec4(1, 0, 0, 1));
        // drawCircle(960, 540, 72, glm::vec4(0, 1, 0, 1));
        // drawEllipse(320, 540, 256, 72, 30.0f, glm::vec4(0, 0.5f, 1, 1));
        // drawTriangle(320, 216, 192, 144, 0.0f, glm::vec4(1, 1, 0, 1));
        // drawLine(128, 72, 1152, 180, 4, glm::vec4(1, 1, 1, 1));
        // int frame = (int)(now * 22.0f);   // 4 frames per second
        // glm::vec4 white(1, 1, 1, 1);
        // drawSprite(testTex, frame, 4, 896, 324, 128, 128, 0.0f, white);                     // normal
        // drawSprite(testTex, frame, 4, 1056, 324, 128, 128, 0.0f, white, true);              // mirrored
        // drawSprite(testTex, frame, 4, 1216, 324, 128, 128, 0.0f, damageTint(white, 0.25f)); // damaged
        // setGrayscale(true);                                                                 // grayscale test
        // drawSprite(testTex, frame, 4, 1056, 160, 128, 128, 0.0f, white);
        // setGrayscale(false);
        // Button testButton = { 640, 600, 260, 70, "START" };   // x, y (center), width, height, label
        // drawButton(testButton);
        // if (isClicked(testButton)) std::cout << "clicked!" << std::endl;

        // ----------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shutdownDraw();
    glfwTerminate();
    return 0;
}