#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;  //to not include glfw in all files
void initInput(GLFWwindow* window);   // call once after creating the window
void updateInput();                  

glm::vec2 mousePos();          
bool mouseClicked();           
bool mouseRightClicked();      

// key is key code masalan GLFW_KEY_W, GLFW_KEY_ESCAPE el howa number
bool keyPressed(int key);      
bool keyHeld(int key);         


bool mouseInRect(float x, float y, float w, float h);

// Closes the game
void requestExit();
