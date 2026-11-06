
// Follow learnopengl: Hello Triangle -> Shaders -> Transformations -> Textures.
#include "Draw.h"
#include "Config.h"   // SCREEN_W, SCREEN_H
#include <glad/glad.h>//opengl functions
#include <glm/gtc/matrix_transform.hpp>//translate rotate etc
#include <glm/gtc/type_ptr.hpp>//gives ptr le a matrixs number
#include <vector>//for std vector
#include <cmath>
#include <iostream>//error print
// stb_image is a library it loads the png into mem
//defining STB_IMAGE_IMPLEMENTATION in 1 .cpp file to include the real code here
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
static void initSprites();
static void shutdownSprites();//implemented here fel a5er
static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 u_Transform;
uniform mat4 projection;   // turns pixels into OpenGL's -1 to 1 range (learnopengl: 2D Game -> Rendering Sprites)
void main()
{
    gl_Position = projection * u_Transform * vec4(aPos, 1.0);
}
)";//el shaders nafs fekret the lab bas 1 color for the whole shape 
//dauser the corners to transform and project and fragment colours the pixels

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


static GLuint shaderProgram = 0;//id of chape shader
static GLint locTransform = -1;//where the transform is in the shader
static GLint locColor = -1;//where the colour is
static GLint locGrayscale = -1;//where greyscale is
static bool grayscaleOn = false;   // set by setGrayscale() and sent to both shaders on every draw


static glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_W, 0.0f, (float)SCREEN_H, -1.0f, 1.0f);//map x from o and 1280 to -1 and 1 same for y. left right bottom top

// One VAO(how to read block) and VBO(block of gpu mem holding corners ). el shapes unit shapes we han transform them
static GLuint squareVAO, squareVBO;
static GLuint triangleVAO, triangleVBO;
static GLuint circleVAO, circleVBO;
static const int CIRCLE_SEGMENTS = 50;      // higher it is smoother the circle

static GLuint compileShader(GLenum type, const char* source) {//takes shader text and turns it into a  program .taken from lab 1
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


static void makeShape(const std::vector<float>& verts, GLuint& vao, GLuint& vbo) {//coordinates of corners and its vao and vbo
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);//get ids
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);//lab 1 stuff
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);//num to gpu
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);//how to read buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void initDraw() {
    initSprites();   
    // build shader program like lab 1
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
    locGrayscale = glGetUniformLocation(shaderProgram, "u_Grayscale");//from lab 2 gets loc of the u slot and stores in local version

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));//select program and send matrix

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//using alpa lel transparency

    //unit square
    makeShape({
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
        }, squareVAO, squareVBO);

    // Unit triangle . this and circle from labs 
    makeShape({
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
        }, triangleVAO, triangleVBO);

    // unit circle
    std::vector<float> circle = { 0.0f, 0.0f, 0.0f };
    for (int i = 0; i <= CIRCLE_SEGMENTS; ++i) {
        float angle = 2.0f * 3.14159265f * (float)i / (float)CIRCLE_SEGMENTS;
        circle.insert(circle.end(), { cosf(angle), sinf(angle), 0.0f });
    }
    makeShape(circle, circleVAO, circleVBO);
}

void shutdownDraw() {//like lab 1 deletes vao and vbos
    shutdownSprites();   // free the image drawing shader and quad
    glDeleteVertexArrays(1, &squareVAO);   glDeleteBuffers(1, &squareVBO);
    glDeleteVertexArrays(1, &triangleVAO); glDeleteBuffers(1, &triangleVBO);
    glDeleteVertexArrays(1, &circleVAO);   glDeleteBuffers(1, &circleVBO);
    glDeleteProgram(shaderProgram);
}


//scale first then rotate then move ffa write opposite for glm
static void drawShape(GLuint vao, GLenum mode, int vertexCount,
    float x, float y, float scaleX, float scaleY, float rotationDeg, glm::vec4 color)
{                                //does nothing
    glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));          // 3. move
    t = glm::rotate(t, glm::radians(rotationDeg), glm::vec3(0.0f, 0.0f, 1.0f));    // 2. rotate
    t = glm::scale(t, glm::vec3(scaleX, scaleY, 1.0f));                            // 1. scale

    glUseProgram(shaderProgram);//select
    glUniformMatrix4fv(locTransform, 1, GL_FALSE, glm::value_ptr(t));//send matric
    glUniform4fv(locColor, 1, glm::value_ptr(color));
    glUniform1i(locGrayscale, grayscaleOn);//colour and greyscale flags
    glBindVertexArray(vao);
    glDrawArrays(mode, 0, vertexCount);//pick shape and draaw it
    glBindVertexArray(0);//unbind
}

            //centre coordinates
void drawRect(float x, float y, float w, float h, float rotation, glm::vec4 color) {//scale is the w and h brc sides are 1
    drawShape(squareVAO, GL_TRIANGLE_FAN, 4, x, y, w, h, rotation, color);
}

void drawCircle(float x, float y, float radius, glm::vec4 color) {
    drawShape(circleVAO, GL_TRIANGLE_FAN, CIRCLE_SEGMENTS + 2, x, y, radius, radius, 0.0f, color);
}

void drawEllipse(float x, float y, float w, float h, float rotation, glm::vec4 color) {
    //div by 2 bec w and h are the full ones and unit circle r is 1
    drawShape(circleVAO, GL_TRIANGLE_FAN, CIRCLE_SEGMENTS + 2, x, y, w / 2.0f, h / 2.0f, rotation, color);
}

void drawTriangle(float x, float y, float w, float h, float rotation, glm::vec4 color) {
    drawShape(triangleVAO, GL_TRIANGLES, 3, x, y, w, h, rotation, color);
}

void drawLine(float x1, float y1, float x2, float y2, float thickness, glm::vec4 color) {
    // A line isa thin rectangle
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrtf(dx * dx + dy * dy);
    float angleDeg = glm::degrees(atan2f(dy, dx));//rad to deg
    drawRect((x1 + x2) / 2.0f, (y1 + y2) / 2.0f, length, thickness, angleDeg, color);
}

glm::vec4 damageTint(glm::vec4 color, float healthFrac) {
    glm::vec4 red(0.9f, 0.05f, 0.05f, color.a);   // keep the original transparency
    float hurt = 1.0f - healthFrac;   // 0 = healthy 1 = almost dead(3akast el health)
    return glm::mix(color, red, hurt * 0.8f);      // 0.8 so it never becomes purely red
}

void setGrayscale(bool on) { grayscaleOn = on; }




//vertex shader runs once for each corner of the rectangle
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
)";//same as shapeone bas also can choose which part of the pic to show el pic 8 frames

// fragment shader runs once every pixel of the rectangle
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
)";//reads colour at each pixel and multiply tint by the colour at uv

static GLuint spriteProgram = 0;         // the compiled sprite shader
static GLuint spriteVAO = 0, spriteVBO = 0;   // the rectangle (with UVs)
static GLint locSpriteTransform = -1;    // the slots in the shader we send values into
static GLint locSpriteFrame = -1;
static GLint locSpriteFrameCount = -1;
static GLint locSpriteFlipX = -1;
static GLint locSpriteTint = -1;
static GLint locSpriteTexture = -1;
static GLint locSpriteGrayscale = -1;

static void initSprites() {
    // same steps as initDraw
    GLuint vs = compileShader(GL_VERTEX_SHADER, spriteVertexSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, spriteFragmentSource);
    spriteProgram = glCreateProgram();
    glAttachShader(spriteProgram, vs);
    glAttachShader(spriteProgram, fs);
    glLinkProgram(spriteProgram);
    glDeleteShader(vs);               
    glDeleteShader(fs);

    locSpriteTransform = glGetUniformLocation(spriteProgram, "u_Transform");
    locSpriteFrame = glGetUniformLocation(spriteProgram, "u_Frame");
    locSpriteFrameCount = glGetUniformLocation(spriteProgram, "u_FrameCount");
    locSpriteFlipX = glGetUniformLocation(spriteProgram, "u_FlipX");
    locSpriteTint = glGetUniformLocation(spriteProgram, "u_Tint");
    locSpriteTexture = glGetUniformLocation(spriteProgram, "u_Texture");
    locSpriteGrayscale = glGetUniformLocation(spriteProgram, "u_Grayscale");
    glUseProgram(spriteProgram);
    glUniformMatrix4fv(glGetUniformLocation(spriteProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    float quad[] = {
        //where it goes on screen and which spot on the png we should show
        //  x      y     z     u     v
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-left  to  bottom left of the pic
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right 
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right   
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f// top left  
    };

    glGenVertexArrays(1, &spriteVAO);  
    glGenBuffers(1, &spriteVBO);         
    glBindVertexArray(spriteVAO);        // start recording into this VAO
    glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);                            // use this VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);  // copy the corners to the GPU

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);//like labs saying el layout badal za7ma keda lazem a know wha these numbers represent

    glBindBuffer(GL_ARRAY_BUFFER, 0); //to not chnge bel8alat
    glBindVertexArray(0);
}

static void shutdownSprites() {//delete
    glDeleteVertexArrays(1, &spriteVAO);
    glDeleteBuffers(1, &spriteVBO);
    glDeleteProgram(spriteProgram);
}

unsigned int loadTexture(const char* path) {//read png gives it to gpu and returns id

    stbi_set_flip_vertically_on_load(true);//flip pic bec origin bottom in opengl in pic its top

    int width, height, channels;         // stb fills these in px size and how many color values per pixel
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);//4 for alpha
    if (!data) {                         
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0;//lab 4 check failure
    }

    GLuint texture;
    glGenTextures(1, &texture);                  // get id from opengl and select
    glBindTexture(GL_TEXTURE_2D, texture);       

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   // when shown smaller
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   // when shown bigger smt for making it not blurry when scaled
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//in case the uv are not within 0to1

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // no padding bw rows fel px

    // copy the pixels to the GPU as rgba each channel is an unsigned byte
                //kind of txt  detail lvl   gpu format          border 0 my format       pxl men el stb
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);  // free cpu gpu has it 5alas
    glBindTexture(GL_TEXTURE_2D, 0);             // unbind
    return texture;
}
void drawSprite(unsigned int texture, int frame, int frameCount,
    float x, float y, float w, float h,
    float rotation, glm::vec4 tint, bool flipX)
{
    if (texture == 0 || frameCount < 1) return;  // nothing to draw 
//like draw shape
    glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));    // 3. move center  to x, y
    t = glm::rotate(t, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));  // 2. rotate (degrees to radians)
    t = glm::scale(t, glm::vec3(w, h, 1.0f));                                     // 1. scale the unit square to w x h

    frame = frame % frameCount;     // wrap like frame 9 men 8 is 1 
    if (frame < 0) frame += frameCount;          // handle negative numbers too

    glUseProgram(spriteProgram); // switch to the sprite shader
    glUniformMatrix4fv(locSpriteTransform, 1, GL_FALSE, glm::value_ptr(t));   // send the transform lel shader
    glUniform1f(locSpriteFrame, (float)frame);     // send the frame number
    glUniform1f(locSpriteFrameCount, (float)frameCount); // send num of frames
    glUniform1f(locSpriteFlipX, flipX ? 1.0f : 0.0f);   // send mirror on/off
    glUniform4fv(locSpriteTint, 1, glm::value_ptr(tint));    // send the tint color
    glUniform1i(locSpriteGrayscale, grayscaleOn);  // send grayscale on/off

    glActiveTexture(GL_TEXTURE0);     // use texture slot 0 fel gpu
    glBindTexture(GL_TEXTURE_2D, texture);   // put our picture in slot 0
    glUniform1i(locSpriteTexture, 0); //tell the shader "u_Texture is in slot 0"

    glBindVertexArray(spriteVAO);  //pick rect
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //draw
    glBindVertexArray(0);         // unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}