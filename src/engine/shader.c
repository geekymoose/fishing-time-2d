#include "shader.h"

#include "engine/assertions.h"
#include "engine/files.h"
#include "engine/log.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 createShaderFromSource(const char* _sources, const int32 _type)
{
    uint32 id = glCreateShader(_type);
    if (id == 0) {
        LOG_ERR("[Shader] Unable to create a shader for type %d", _type);
        return 0;
    }

    glShaderSource(id, 1, &_sources, NULL);
    glCompileShader(id);

    int32 success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        char strlog[256];
        glGetShaderInfoLog(id, 255, NULL, strlog);
        LOG_ERR("[Shader] Unable to compile shader %d: %s", _type, strlog);
        return 0;
    }

    return id;
}

uint32 createShadeFromFile(const char* _path, const int32 _type)
{
    LOG_INFO("[Shader] Create shader from file %s", _path);

    char* filebuffer = readFileBuffer(_path);
    if (filebuffer == NULL) {
        LOG_ERR("[Shader] Unable to create shader from file %s", _path);
        ASSERT_MSG(FALSE, "[Shader] Unable to create shader from file %s", _path);
        return 0;
    }

    uint32 id = createShaderFromSource(filebuffer, _type);
    free(filebuffer);

    return id;
}

uint32 createShaderProgram(const uint32 _idVertexShader, const uint32 _idFragShader)
{
    uint32 id = glCreateProgram();
    if (id == 0) {
        LOG_ERR("[Shader] Unable to create shader program");
        return 0;
    }
    glAttachShader(id, _idVertexShader);
    glAttachShader(id, _idFragShader);

    glLinkProgram(id);
    int32 result;
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
        char loginfo[256];
        glGetProgramInfoLog(id, 255, NULL, loginfo);
        LOG_ERR("[Shader] Unable to link shader program %s", loginfo);
        return 0;
    }

    return id;
}

uint32 createShaderProgramFromFile(const char* _vertexPath, const char* _fragPath)
{
    uint32 idVertex = createShadeFromFile(_vertexPath, GL_VERTEX_SHADER);
    uint32 idFrag = createShadeFromFile(_fragPath, GL_FRAGMENT_SHADER);
    return createShaderProgram(idVertex, idFrag);
}

void setShaderProgramUniform(uint32 _shaderID, const char* _name, float _x, float _y)
{
    glUseProgram(_shaderID);
    glUniform2f(glGetUniformLocation(_shaderID, _name), _x, _y);
}
