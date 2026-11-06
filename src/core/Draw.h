#pragma once
//every picture in the game is built from this

#include <glm/glm.hpp>

void initDraw(); // called oncefel awel builds shapes and does the shaders
void shutdownDraw();
void drawRect(float x, float y, float w, float h, float rotation, glm::vec4 color);//x y is the center and rot in degrees
void drawCircle(float x, float y, float radius, glm::vec4 color);
void drawEllipse(float x, float y, float w, float h, float rotation, glm::vec4 color);
void drawTriangle(float x, float y, float w, float h, float rotation, glm::vec4 color);
void drawLine(float x1, float y1, float x2, float y2, float thickness, glm::vec4 color);
//load a pic at startup texture id returnedd 0 law fail 
unsigned int loadTexture(const char* path);

void drawSprite(unsigned int texture, int frame, int frameCount,
                float x, float y, float w, float h,
                float rotation, glm::vec4 tint, bool flipX = false);// draw one frame of a sprite strip (homa side by side just one row


glm::vec4 damageTint(glm::vec4 color, float healthFrac);//for inc red as character is dying
void setGrayscale(bool on);//for locked cards
