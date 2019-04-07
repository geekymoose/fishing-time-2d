#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow * createWindowGLFW(const int width, const int height, const char* title);

GLuint createShaderFromSrc(const char*, const GLenum);
GLuint createShadeFromFile(const char*, const GLenum);

GLuint createShaderProgram(const GLuint _idVertexShader, const GLuint _idFragShader);
GLuint createShaderProgramFromFile(const char* _vertexPath, const char* _fragPath);

