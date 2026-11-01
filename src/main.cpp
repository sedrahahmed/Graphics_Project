// main.cpp: opens the window and runs the game loop. Owner: Person A
#include <glad/glad.h>        // lazem before GLFW and its used to find opengl functions from our computer so we can call them
#include <GLFW/glfw3.h>//helper library to opengl and its used to read keyboard masalam
#include <iostream>

#include "Config.h"
#include "Draw.h"
#include "Input.h"
#include "StateManager.h"
#include "Button.h"
//GUYS 3AYZEEN N CHANGE VARIABLE NAMES WE NESHEEL EXTRA COMMENTS WHEN WERE DONE
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
int main() {
    glfwInit(); //turn on the library and we call it before using any function taba3ha
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //picking version opengl
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //we want the modern version
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   // required on Mac
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Carnival of the Cursed", nullptr, nullptr);//create and open window (returns pointer and thats how well use el window)
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//pick this window to draw 3aleh
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);//3ashan lama n resize el window run the function

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //here glad finds all our opengl commands 
        std::cout << "Failed to load GLAD" << std::endl;//cant draw if not found
        return -1;
    }

    int w, h;                                  // Mac Retina fix
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);//tell opengl how many pixels to draw on the window

    glEnable(GL_BLEND);                            // to make png with transparent bg appear actually transparent mesh filled in
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    initDraw(); 
    unsigned int testTex = loadTexture(ASSET_DIR "characters/test_sprite.png"); //load the pic 3ashan n paste it onto the shape (da hena the texture part)
    initInput(window);// waiting for input 

    // TODO later: initText(ASSET_DIR "fonts/...", 48); initSound();

    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) { //kol loop in a frame
        float now = (float)glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;//ad eh time passed men last frame

        updateInput();
        updateCurrentState(dt);//getting inputs and udate the game like law smt moved

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

        glfwSwapBuffers(window); //draw my new frame on the window
        glfwPollEvents(); //check for if we preessed a key(input?)
    }

    shutdownDraw();
    glfwTerminate();
    return 0; //just to close baraga3 mem and close window
}