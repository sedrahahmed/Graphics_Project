#pragma once
// Shader.h: compiling shaders and setting uniforms. Used inside Draw.cpp and Text.cpp.
// Owner: Person A

#include <glm/glm.hpp>

// Compiles + links a vertex and fragment shader. Prints errors to the console.
unsigned int createShaderProgram(const char* vertexSrc, const char* fragmentSrc);

// Set a uniform on the shader that is currently in use (glUseProgram).
void setUniformMat4(unsigned int shader, const char* name, const glm::mat4& value);
void setUniformVec4(unsigned int shader, const char* name, const glm::vec4& value);
void setUniformVec2(unsigned int shader, const char* name, const glm::vec2& value);
void setUniformInt (unsigned int shader, const char* name, int value);
void setUniformBool(unsigned int shader, const char* name, bool value);
