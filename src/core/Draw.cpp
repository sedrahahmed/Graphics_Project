// Draw.cpp. Owner: Person A
// Follow learnopengl: Hello Triangle -> Shaders -> Transformations -> Textures.
#include "Draw.h"
#include "Config.h"   // SCREEN_W, SCREEN_H
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
#include <iostream>
// stb_image is a "single header" library: it contains the whole loader inside the .h file.
// Defining STB_IMAGE_IMPLEMENTATION in EXACTLY ONE .cpp file tells it to include the real code here.
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// Sprite functions live at the bottom of this file; we declare them here so initDraw/shutdownDraw can call them
static void initSprites();
static void shutdownSprites();
// ---------- Shaders (same idea as the lab, but ONE color for the whole shape) ----------
static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 u_Transform;
uniform mat4 projection;   // turns pixels into OpenGL's -1 to 1 range (learnopengl: 2D Game -> Rendering Sprites)
void main()
{
    gl_Position = projection * u_Transform * vec4(aPos, 1.0);
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 u_Color;   // r, g, b, alpha
uniform bool u_Grayscale;
void main()
{
    FragColor = u_Color;
    if (u_Grayscale) {   // learnopengl: Framebuffers -> Post-processing -> Grayscale
        float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
        FragColor = vec4(average, average, average, FragColor.a);   // keep alpha so see-through stays see-through
    }
}
)";

// ---------- Globals ----------
static GLuint shaderProgram = 0;
static GLint locTransform = -1;
static GLint locColor = -1;
static GLint locGrayscale = -1;
static bool grayscaleOn = false;   // set by setGrayscale(), sent to both shaders on every draw

// Pixel coordinates: (0,0) = bottom-left, (SCREEN_W, SCREEN_H) = top-right.
// learnopengl uses ortho(0, 800, 600, 0) for a top-left origin; we swap the last two for bottom-left.
static glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_W, 0.0f, (float)SCREEN_H, -1.0f, 1.0f);

// One VAO/VBO per base shape. Each is a "unit" shape that we scale/rotate/move later.
static GLuint squareVAO, squareVBO;
static GLuint triangleVAO, triangleVBO;
static GLuint circleVAO, circleVBO;
static const int CIRCLE_SEGMENTS = 50;      // higher = smoother circle

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

// Uploads a list of x,y,z floats and returns the VAO/VBO for it
static void makeShape(const std::vector<float>& verts, GLuint& vao, GLuint& vbo) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void initDraw() {
    initSprites();   // also set up the image-drawing shader and quad
    // Build shader program
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    locTransform = glGetUniformLocation(shaderProgram, "u_Transform");
    locColor = glGetUniformLocation(shaderProgram, "u_Color");
    locGrayscale = glGetUniformLocation(shaderProgram, "u_Grayscale");

    // The projection never changes, so send it once (like learnopengl does in Init)
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Lets the alpha (4th number of the color) make shapes see-through
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Unit square: 1 x 1, centered on (0,0). Drawn as a triangle fan.
    makeShape({
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
        }, squareVAO, squareVBO);

    // Unit triangle: 1 wide, 1 tall, centered on (0,0)
    makeShape({
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
        }, triangleVAO, triangleVBO);

    // Unit circle: radius 1, triangle fan (center point + points around the edge)
    std::vector<float> circle = { 0.0f, 0.0f, 0.0f };
    for (int i = 0; i <= CIRCLE_SEGMENTS; ++i) {
        float angle = 2.0f * 3.14159265f * (float)i / (float)CIRCLE_SEGMENTS;
        circle.insert(circle.end(), { cosf(angle), sinf(angle), 0.0f });
    }
    makeShape(circle, circleVAO, circleVBO);
}

void shutdownDraw() {
    shutdownSprites();   // free the image-drawing shader and quad
    glDeleteVertexArrays(1, &squareVAO);   glDeleteBuffers(1, &squareVBO);
    glDeleteVertexArrays(1, &triangleVAO); glDeleteBuffers(1, &triangleVBO);
    glDeleteVertexArrays(1, &circleVAO);   glDeleteBuffers(1, &circleVBO);
    glDeleteProgram(shaderProgram);
}

// ---------- The one helper that everything uses ----------
// Order matters: scale first, then rotate, then move. In glm you write it in reverse.
static void drawShape(GLuint vao, GLenum mode, int vertexCount,
    float x, float y, float scaleX, float scaleY, float rotationDeg, glm::vec4 color)
{
    glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));          // 3. move
    t = glm::rotate(t, glm::radians(rotationDeg), glm::vec3(0.0f, 0.0f, 1.0f));    // 2. rotate
    t = glm::scale(t, glm::vec3(scaleX, scaleY, 1.0f));                            // 1. scale

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(locTransform, 1, GL_FALSE, glm::value_ptr(t));
    glUniform4fv(locColor, 1, glm::value_ptr(color));
    glUniform1i(locGrayscale, grayscaleOn);
    glBindVertexArray(vao);
    glDrawArrays(mode, 0, vertexCount);
    glBindVertexArray(0);
}

// ---------- The 5 functions you asked for ----------
void drawRect(float x, float y, float w, float h, float rotation, glm::vec4 color) {
    drawShape(squareVAO, GL_TRIANGLE_FAN, 4, x, y, w, h, rotation, color);
}

void drawCircle(float x, float y, float radius, glm::vec4 color) {
    drawShape(circleVAO, GL_TRIANGLE_FAN, CIRCLE_SEGMENTS + 2, x, y, radius, radius, 0.0f, color);
}

void drawEllipse(float x, float y, float w, float h, float rotation, glm::vec4 color) {
    // w and h are the full width/height, the unit circle has radius 1, so we use half
    drawShape(circleVAO, GL_TRIANGLE_FAN, CIRCLE_SEGMENTS + 2, x, y, w / 2.0f, h / 2.0f, rotation, color);
}

void drawTriangle(float x, float y, float w, float h, float rotation, glm::vec4 color) {
    drawShape(triangleVAO, GL_TRIANGLES, 3, x, y, w, h, rotation, color);
}

void drawLine(float x1, float y1, float x2, float y2, float thickness, glm::vec4 color) {
    // A line is just a thin rectangle: put it at the midpoint, as long as the gap,
    // and tilt it by the angle between the two points.
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrtf(dx * dx + dy * dy);
    float angleDeg = glm::degrees(atan2f(dy, dx));
    drawRect((x1 + x2) / 2.0f, (y1 + y2) / 2.0f, length, thickness, angleDeg, color);
}

glm::vec4 damageTint(glm::vec4 color, float healthFrac) {
    glm::vec4 red(0.9f, 0.05f, 0.05f, color.a);   // keep the original transparency
    float hurt = 1.0f - healthFrac;                // 0 = healthy, 1 = almost dead
    return glm::mix(color, red, hurt * 0.8f);      // 0.8 so it never becomes a flat red blob
}

void setGrayscale(bool on) { grayscaleOn = on; }

// =====================================================================================
//                                  SPRITES (PNG images)
// =====================================================================================
// A sprite is just a rectangle with a picture glued on it. The picture is a "texture".
// Every corner of the rectangle gets a UV coordinate:
//   UV = (0,0) is the bottom-left of the picture, UV = (1,1) is the top-right.
// The GPU then looks up the right pixel of the picture for every pixel of the rectangle.

// ---------- Sprite shaders ----------
// Vertex shader: runs once per corner of the rectangle
static const char* spriteVertexSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;      // corner position (the unit square, -0.5 to 0.5)
layout (location = 1) in vec2 aUV;       // which spot of the picture this corner points at (0 to 1)

uniform mat4  u_Transform;               // move/rotate/scale, exactly like drawShape
uniform mat4  projection;                // pixels -> OpenGL's -1 to 1 range
uniform float u_Frame;                  // which frame of the strip to show (0, 1, 2, ...)
uniform float u_FrameCount;              // how many frames the strip has
uniform float u_FlipX;                   // 0 = normal, 1 = mirrored left-right

out vec2 vUV;                            // sent to the fragment shader

void main()
{
    gl_Position = projection * u_Transform * vec4(aPos, 1.0);   // put the corner in the right place on screen

    float u = aUV.x;                               // horizontal spot inside ONE frame (0 to 1)
    if (u_FlipX > 0.5) u = 1.0 - u;                // mirror: 0 becomes 1 and 1 becomes 0

    // The strip has frameCount frames side by side. Each is 1/frameCount of the width.
    // (u + frame) picks the frame, dividing by frameCount shrinks it into 0-1 of the whole image.
    vUV = vec2((u + u_Frame) / u_FrameCount, aUV.y);
}
)";

// Fragment shader: runs once per pixel of the rectangle
static const char* spriteFragmentSource = R"(
#version 330 core
in vec2 vUV;                             // the picture spot for this pixel
out vec4 FragColor;                      // the final pixel color

uniform sampler2D u_Texture;             // the picture
uniform vec4 u_Tint;                     // color multiplied with the picture (white = unchanged)
uniform bool u_Grayscale;                // true = draw in gray (locked cards)

void main()
{
    FragColor = texture(u_Texture, vUV) * u_Tint;  // read the picture's pixel and tint it
    if (u_Grayscale) {                             // same grayscale as the shape shader
        float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
        FragColor = vec4(average, average, average, FragColor.a);
    }
}
)";

// ---------- Sprite globals ----------
static GLuint spriteProgram = 0;         // the compiled sprite shader
static GLuint spriteVAO = 0, spriteVBO = 0;   // the rectangle (with UVs)
static GLint locSpriteTransform = -1;    // the "slots" in the shader we send values into
static GLint locSpriteFrame = -1;
static GLint locSpriteFrameCount = -1;
static GLint locSpriteFlipX = -1;
static GLint locSpriteTint = -1;
static GLint locSpriteTexture = -1;
static GLint locSpriteGrayscale = -1;

static void initSprites() {
    // Compile both shaders and link them into one program (same steps as initDraw)
    GLuint vs = compileShader(GL_VERTEX_SHADER, spriteVertexSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, spriteFragmentSource);
    spriteProgram = glCreateProgram();
    glAttachShader(spriteProgram, vs);
    glAttachShader(spriteProgram, fs);
    glLinkProgram(spriteProgram);
    glDeleteShader(vs);                  // shaders are copied into the program, so we can delete these
    glDeleteShader(fs);

    // Ask the shader where each uniform lives so we can fill them in later
    locSpriteTransform = glGetUniformLocation(spriteProgram, "u_Transform");
    locSpriteFrame = glGetUniformLocation(spriteProgram, "u_Frame");
    locSpriteFrameCount = glGetUniformLocation(spriteProgram, "u_FrameCount");
    locSpriteFlipX = glGetUniformLocation(spriteProgram, "u_FlipX");
    locSpriteTint = glGetUniformLocation(spriteProgram, "u_Tint");
    locSpriteTexture = glGetUniformLocation(spriteProgram, "u_Texture");
    locSpriteGrayscale = glGetUniformLocation(spriteProgram, "u_Grayscale");

    // Send the projection once, same as the shape shader
    glUseProgram(spriteProgram);
    glUniformMatrix4fv(glGetUniformLocation(spriteProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // The unit square again, but now each corner is: x, y, z, u, v (5 floats)
    // Corner order goes around the square so GL_TRIANGLE_FAN fills it.
    float quad[] = {
        //  x      y     z     u     v
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom-left  -> bottom-left of the picture
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom-right -> bottom-right of the picture
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top-right    -> top-right of the picture
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // top-left     -> top-left of the picture
    };

    glGenVertexArrays(1, &spriteVAO);    // make the VAO (remembers how to read the data)
    glGenBuffers(1, &spriteVBO);         // make the VBO (holds the data on the GPU)
    glBindVertexArray(spriteVAO);        // start recording into this VAO
    glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);                            // use this VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);  // copy the corners to the GPU

    // Attribute 0 = position: 3 floats, each corner is 5 floats long (stride), starts at byte 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Attribute 1 = UV: 2 floats, starts after the 3 position floats
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);    // unbind so we don't change them by accident
    glBindVertexArray(0);
}

static void shutdownSprites() {
    glDeleteVertexArrays(1, &spriteVAO);
    glDeleteBuffers(1, &spriteVBO);
    glDeleteProgram(spriteProgram);
}

// ---------- loadTexture ----------
// Reads a PNG file from disk, uploads it to the GPU, returns its ID. Returns 0 if it failed.
unsigned int loadTexture(const char* path) {
    // OpenGL thinks (0,0) is the BOTTOM-left, but image files store the TOP row first.
    // This flips the image while loading so it isn't upside down.
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;         // stb fills these in: image size and channels in the FILE
    // The last "4" forces RGBA (red, green, blue, alpha) no matter what the file has,
    // so we always know the pixel layout. Piskel PNGs keep transparency in alpha.
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
    if (!data) {                         // file missing / wrong path / not a real PNG
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);                  // ask OpenGL for a texture ID
    glBindTexture(GL_TEXTURE_2D, texture);       // "everything below is about this texture"

    // NEAREST = no blur when scaled. Keeps pixel art crisp (LINEAR would smudge it).
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   // when shown smaller
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   // when shown bigger
    // CLAMP_TO_EDGE = never wrap around to the opposite side of the picture at the borders
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);       // pixel rows are tightly packed (no padding bytes)

    // Copy the pixels to the GPU. Stored as RGBA, our data is RGBA, each channel is an unsigned byte.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);                       // the GPU has its own copy now, free the CPU one
    glBindTexture(GL_TEXTURE_2D, 0);             // unbind
    return texture;
}

// ---------- drawSprite ----------
void drawSprite(unsigned int texture, int frame, int frameCount,
    float x, float y, float w, float h,
    float rotation, glm::vec4 tint, bool flipX)
{
    if (texture == 0 || frameCount < 1) return;  // nothing to draw (failed load or bad frameCount)

    // Same transform recipe as drawShape: scale first, then rotate, then move (written in reverse)
    glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));          // 3. move to x, y
    t = glm::rotate(t, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));       // 2. rotate (degrees -> radians)
    t = glm::scale(t, glm::vec3(w, h, 1.0f));                                      // 1. stretch the unit square to w x h

    frame = frame % frameCount;                  // wrap so frame 4 of 4 goes back to 0
    if (frame < 0) frame += frameCount;          // handle negative numbers too

    glUseProgram(spriteProgram);                 // switch to the sprite shader
    glUniformMatrix4fv(locSpriteTransform, 1, GL_FALSE, glm::value_ptr(t));   // send the transform
    glUniform1f(locSpriteFrame, (float)frame);                                // send the frame number
    glUniform1f(locSpriteFrameCount, (float)frameCount);                      // send how many frames there are
    glUniform1f(locSpriteFlipX, flipX ? 1.0f : 0.0f);                         // send mirror on/off
    glUniform4fv(locSpriteTint, 1, glm::value_ptr(tint));                     // send the tint color
    glUniform1i(locSpriteGrayscale, grayscaleOn);                             // send grayscale on/off

    glActiveTexture(GL_TEXTURE0);                // use texture slot 0
    glBindTexture(GL_TEXTURE_2D, texture);       // put our picture in slot 0
    glUniform1i(locSpriteTexture, 0);            // tell the shader "u_Texture is in slot 0"

    glBindVertexArray(spriteVAO);                // pick the rectangle
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);         // draw its 4 corners as a filled square
    glBindVertexArray(0);                        // unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}