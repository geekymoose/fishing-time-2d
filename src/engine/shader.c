#include "shader.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include "engine/log.h"


uint32 createShaderFromSource(const char * _sources, const int32 _type)
{
    uint32 id = glCreateShader(_type);
    if(id == 0)
    {
        LOG_ERR("[Shader] Unable to create a shader for type %d", _type);
        return 0;
    }

    glShaderSource(id, 1, &_sources, NULL);
    glCompileShader(id);

    int32 success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(success != GL_TRUE)
    {
        char strlog[256];
        glGetShaderInfoLog(id, 255, NULL, strlog);
        LOG_ERR("[Shader] Unable to compile shader %d: %s", _type, strlog);
        return 0;
    }

    return id;
}

uint32 createShadeFromFile(const char* _path, const int32 _type)
{
    LOG_INFO("[Shader] Create shader %s", _path);

    FILE * file;
    file = fopen(_path, "r");
    if(file == NULL)
    {
        LOG_ERR("[Shader] Unable to open the shader %s", _path);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);
    rewind(file);

    char * filebuffer = NULL;
    filebuffer = malloc((file_size + 1) * sizeof(*filebuffer)); // +1 for '\0'
    if(filebuffer == NULL)
    {
        LOG_ERR("[Shader] Unable to read %s (malloc %d bytes failed)", _path, filebuffer);
        return 0;
    }

    size_t elementsRead = fread(filebuffer, sizeof(char), file_size, file);
    if(elementsRead != file_size)
    {
        LOG_ERR("[Shader] Unable to read %s", _path);
        free(filebuffer);
        return 0;
    }
    filebuffer[file_size] = '\0';
    uint32 id = createShaderFromSource(filebuffer, _type);

    free(filebuffer);
    fclose(file);

    return id;
}

uint32 createShaderProgram(const uint32 _idVertexShader, const uint32 _idFragShader)
{
    uint32 id = glCreateProgram();
    if(id == 0)
    {
        LOG_ERR("[Shader] Unable to create shader program");
        return 0;
    }
    glAttachShader(id, _idVertexShader);
    glAttachShader(id, _idFragShader);

    glLinkProgram(id);
    int32 result;
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    if(result != GL_TRUE)
    {
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


