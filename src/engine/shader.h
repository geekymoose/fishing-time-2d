#pragma once

#include "engine/types.h"

uint32 createShaderFromSource(const char *, const uint32);
uint32 createShadeFromFile(const char *, const uint32);

uint32 createShaderProgram(const uint32 _idVertexShader, const uint32 _idFragShader);
uint32 createShaderProgramFromFile(const char * _vertexPath, const char * _fragPath);

void setShaderProgramUniform(uint32 _shaderID, const char * _name, float _x, float _y);

