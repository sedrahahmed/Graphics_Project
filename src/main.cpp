//opens the window and runs the game loop
#include <glad/glad.h>        // lazem before GLFW and its used to find opengl functions from our computer so we can call them
#include <GLFW/glfw3.h>//helper library to opengl and its used to read keyboard masalam
#include <iostream>
#include "CharacterDraw.h"
#include "Config.h"
#include "Draw.h"
#include "Input.h"
#include "StateManager.h"
#include "Button.h"
#include "Text.h"
#include "Sound.h"
#include "Background.h"
#include "Environment.h"
#include "MainMenu.h"

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
    initCharacterTexture();
    initBackground();
    initMainMenu();
    initEnvironment();
    initText(ASSET_DIR "fonts/carnival.ttf",48);
    unsigned int testTex = loadTexture(ASSET_DIR "characters/test_sprite.png"); //load the pic 3ashan n paste it onto the shape (da hena the texture part)
    initInput(window);// waiting for input 

    initSound();
    playMusic();

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

        glfwSwapBuffers(window); //draw my new frame on the window
        glfwPollEvents(); //check for if we preessed a key(input?)
    }

    shutdownSound();
    shutdownDraw();
    glfwTerminate();
    return 0; //just to close baraga3 mem and close window
}