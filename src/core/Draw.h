#pragma once
#include <glm/glm.hpp>

void initDraw();   // call once at startup: builds shapes, compiles shaders

// x, y = center of the shape. rotation in degrees.
void drawRect(float x, float y, float w, float h, float rotation, glm::vec4 color);
void drawCircle(float x, float y, float radius, glm::vec4 color);
void drawEllipse(float x, float y, float w, float h, float rotation, glm::vec4 color);
void drawTriangle(float x, float y, float w, float h, float rotation, glm::vec4 color);
void drawLine(float x1, float y1, float x2, float y2, float thickness, glm::vec4 color);

// Turn on for locked player cards: everything drawn becomes gray
void setGrayscale(bool on);