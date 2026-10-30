#pragma once
// Draw.h: the drawing toolkit. Every picture in the game is built from these.
// Owner: Person A. Shared contract: don't change without telling the team.

#include <glm/glm.hpp>

void initDraw();   // call once at startup: builds shapes, compiles shaders

// ---------- Shapes ----------
// x, y = center of the shape. rotation in degrees.
void drawRect(float x, float y, float w, float h, float rotation, glm::vec4 color);
void drawCircle(float x, float y, float radius, glm::vec4 color);
void drawEllipse(float x, float y, float w, float h, float rotation, glm::vec4 color);
void drawTriangle(float x, float y, float w, float h, float rotation, glm::vec4 color);
void drawLine(float x1, float y1, float x2, float y2, float thickness, glm::vec4 color);

// ---------- Sprites (PNG images from Piskel) ----------
// Load a PNG once at startup. Returns its texture ID (0 if loading failed).
unsigned int loadTexture(const char* path);

// Draw one frame of a sprite strip (frames side by side, 1 row).
// x, y = center. tint is multiplied with the image (white = normal colors).
// flipX = true mirrors the image (villains face left).
void drawSprite(unsigned int texture, int frame, int frameCount,
                float x, float y, float w, float h,
                float rotation, glm::vec4 tint, bool flipX = false);

// ---------- Color helpers ----------
// Damage tint: blends a color toward red as health drops.
// healthFrac = 1.0 -> original color, 0.0 -> mostly red.
glm::vec4 damageTint(glm::vec4 color, float healthFrac);

// Turn on for locked player cards: everything drawn becomes gray.
// Remember to turn it off again after drawing the card.
void setGrayscale(bool on);
