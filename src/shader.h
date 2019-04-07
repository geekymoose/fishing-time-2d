#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint createShaderFromSource(const char*, const GLenum);
GLuint createShadeFromFile(const char*, const GLenum);

GLuint createShaderProgram(const GLuint _idVertexShader, const GLuint _idFragShader);
GLuint createShaderProgramFromFile(const char* _vertexPath, const char* _fragPath);

void setShaderProgramUniform(GLuint _shaderID, const char* _name, float _x, float _y);


