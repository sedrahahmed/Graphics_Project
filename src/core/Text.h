#pragma once
#include <string>
#include <glm/glm.hpp>

void initText(const char* fontPath, int pixelSize);// load the font once at startup
void drawText(const std::string& text, float x, float y, float scale, glm::vec4 color);
float textWidth(const std::string& text, float scale);//how wide a word will be
