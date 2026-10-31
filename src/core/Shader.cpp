// Shader.cpp. Owner: Person A
// Follows learnopengl: Getting started -> Shaders -> "Our own shader class".
// learnopengl wraps this in a Shader class; we use plain functions instead,
// and the shader code is passed in as text (not read from a file).
//
// The big picture:
//   1. A shader is a small program that runs on the GPU.
//   2. The VERTEX shader runs once per corner (vertex) and decides WHERE it goes on screen.
//   3. The FRAGMENT shader runs once per pixel and decides what COLOR it is.
//   4. We compile both, then LINK them into one "shader program" that we can use with glUseProgram.
#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>   // glm::value_ptr: gives OpenGL a pointer to a matrix's numbers
#include <iostream>
#include <string>

// ---------- Error checking (learnopengl: checkCompileErrors) ----------
// OpenGL doesn't stop the program if a shader has a typo; it just fails silently.
// So after compiling/linking we ASK it if it worked, and print the error message if not.
// type is "VERTEX", "FRAGMENT" or "PROGRAM" (only used in the printed message).
static void checkCompileErrors(unsigned int id, const char* type) {
    int success;
    char infoLog[1024];
    if (std::string(type) != "PROGRAM") {
        // A single shader: did it COMPILE?
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
    else {
        // The whole program: did the two shaders LINK together?
        // (e.g. fails if the vertex shader's "out" doesn't match the fragment shader's "in")
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
        }
    }
}

// ---------- Build a shader program ----------
// Returns the program's ID. Use it later with glUseProgram(id) and the setUniform functions below.
unsigned int createShaderProgram(const char* vertexSrc, const char* fragmentSrc) {
    // 1. Vertex shader: create an empty shader, give it the source code, compile it
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSrc, nullptr);   // 1 = we pass one string
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // 2. Fragment shader: same steps
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSrc, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // 3. Shader program: attach both shaders and link them into one program
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    checkCompileErrors(program, "PROGRAM");

    // 4. The program now has its own copy, so the separate shaders aren't needed anymore
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

// ---------- Uniforms (learnopengl: setBool, setInt, setVec2, setVec4, setMat4) ----------
// A uniform is a value we send from C++ to the shader, like "uniform vec4 u_Color;".
// Steps every time:
//   1. glGetUniformLocation: find WHERE the uniform lives in the program (by its name)
//   2. glUniform...: send the value to that location
// IMPORTANT: glUniform sends to the program that is currently in use,
// so call glUseProgram(shader) BEFORE these.
// If the name is wrong (or the shader never uses it), the location is -1 and OpenGL ignores it.

// mat4 = a 4x4 matrix (transform, projection)
void setUniformMat4(unsigned int shader, const char* name, const glm::mat4& value) {
    // 1 = one matrix, GL_FALSE = don't transpose (glm already stores it the way OpenGL wants)
    glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, glm::value_ptr(value));
}

// vec4 = 4 floats (a color: r, g, b, alpha)
void setUniformVec4(unsigned int shader, const char* name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(shader, name), 1, glm::value_ptr(value));
}

// vec2 = 2 floats (a position or size)
void setUniformVec2(unsigned int shader, const char* name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(shader, name), 1, glm::value_ptr(value));
}

// int (also used to tell a sampler2D which texture slot to read: 0, 1, 2, ...)
void setUniformInt(unsigned int shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader, name), value);
}

// bool: GLSL has no special function for bools, so it's sent as an int (0 = false, 1 = true)
void setUniformBool(unsigned int shader, const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shader, name), (int)value);
}
