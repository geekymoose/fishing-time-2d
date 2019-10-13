#pragma once

#include "engine/types.h"

uint32 createShaderFromSource(const char * _source, const int32 _type);
uint32 createShadeFromFile(const char * _path, const int32 _type);

uint32 createShaderProgram(const uint32 _idVertexShader, const uint32 _idFragShader);
uint32 createShaderProgramFromFile(const char * _vertexPath, const char * _fragPath);

void setShaderProgramUniform(uint32 _shaderID, const char * _name, float _x, float _y);

