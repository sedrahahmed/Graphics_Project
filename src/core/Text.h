#pragma once
// Text.h: drawing text (menus, subtitles, coin counter).
// Owner: Person D (learnopengl: In Practice -> Text Rendering, FreeType)

#include <string>
#include <glm/glm.hpp>

// Call once at startup. fontPath e.g. ASSET_DIR "fonts/carnival.ttf"
void initText(const char* fontPath, int pixelSize);

// x, y = bottom-left of the text. scale 1.0 = the pixelSize given to initText.
void drawText(const std::string& text, float x, float y, float scale, glm::vec4 color);

// Width in pixels, for centering: x = centerX - textWidth(text, scale) / 2
float textWidth(const std::string& text, float scale);
