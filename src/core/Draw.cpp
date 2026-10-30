// Draw.cpp. Owner: Person A
// Follow learnopengl: Hello Triangle -> Shaders -> Transformations -> Textures.
#include "Draw.h"

void initDraw() { }   // TODO: build square/triangle/circle VAOs, compile shader, set projection

void drawRect(float x, float y, float w, float h, float rotation, glm::vec4 color) { }
void drawCircle(float x, float y, float radius, glm::vec4 color) { }
void drawEllipse(float x, float y, float w, float h, float rotation, glm::vec4 color) { }
void drawTriangle(float x, float y, float w, float h, float rotation, glm::vec4 color) { }
void drawLine(float x1, float y1, float x2, float y2, float thickness, glm::vec4 color) { }

unsigned int loadTexture(const char* path) { return 0; }   // TODO: stb_image
void drawSprite(unsigned int texture, int frame, int frameCount,
                float x, float y, float w, float h,
                float rotation, glm::vec4 tint, bool flipX) { }

glm::vec4 damageTint(glm::vec4 color, float healthFrac) {
    glm::vec4 red(0.9f, 0.05f, 0.05f, color.a);   // keep the original transparency
    float hurt = 1.0f - healthFrac;                // 0 = healthy, 1 = almost dead
    return glm::mix(color, red, hurt * 0.8f);      // 0.8 so it never becomes a flat red blob
}

void setGrayscale(bool on) { }
