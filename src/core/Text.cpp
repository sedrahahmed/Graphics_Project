
//A font file (.ttf) describes letter shapes, not pictures. 
//FreeType turns each letter into a small greyscale image (a “glyph”).
//At startup, you turn each of the 128 basic characters (letters, numbers, symbols) into a texture.
//To draw a word, you go letter by letter: draw a small rectangle in the right place with that letter’s texture on it, then move right by the letter’s width.
#include "Text.h"
#include "Draw.h"
#include <ft2build.h>
#include <glad/glad.h>
#include FT_FREETYPE_H
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include "Config.h"

struct Character {
    unsigned int TextureID;  //the letter’s picture on the GPU
    glm::ivec2   Size;    
    glm::ivec2   Bearing;    //how far the letter sits from the cursor and from the baseline
    unsigned int Advance;    //how far to move right before the next letter
};


static std::map<char, Character> Characters;
static unsigned int textVAO, textVBO; //for the rectangle
static unsigned int textShader;
static int locProjection, locTextColor, locTextSampler;

//shaders
static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}  
)";

//We combine both the position and texture coordinate data into one vec4. 
//The vertex shader multiplies the coordinates with a projection matrix and forwards the texture coordinates to the fragment shader
static const char* fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
)";

static GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error:\n" << infoLog << std::endl;
    }
    return shader;
}

void initText(const char* fontPath, int pixelSize) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    textShader = glCreateProgram();
    glAttachShader(textShader, vertexShader);
    glAttachShader(textShader, fragmentShader);
    glLinkProgram(textShader);
    glDeleteShader(vertexShader);//delete the separate shaders
    glDeleteShader(fragmentShader);
    locTextColor = glGetUniformLocation(textShader, "textColor");
    locTextSampler = glGetUniformLocation(textShader, "text");
    locProjection = glGetUniformLocation(textShader, "projection");
    glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_W, 0.0f, (float)SCREEN_H);
    glUseProgram(textShader);
    glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(projection));
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft,fontPath, 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl << fontPath << std::endl;  
            return;
        }
    FT_Set_Pixel_Sizes(face, 0, pixelSize); //Set the font size
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Disable byte-alignment restriction
    for (unsigned char c = 0; c < 128; c++){
        //Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        //Generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        //Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       
        Character character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
    }
void drawText(const std::string& text, float x, float y, float scale, glm::vec4 color) {
    glUseProgram(textShader);
    glUniform3f(locTextColor, color.r, color.g, color.b);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(locTextSampler, 0);
    glBindVertexArray(textVAO);
    for (char c : text) {
        Character ch = Characters[c];
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale; //Bitshift by 6 to get value in pixels
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
 }

float textWidth(const std::string& text, float scale) {
    float width = 0.0f;
    for (char c : text) {
        Character ch = Characters[c];
        width += (ch.Advance >> 6) * scale;
    }
    return width;
}
