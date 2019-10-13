#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include "engine/log.h"


GLuint createShaderFromSource(const char* _sources, const GLenum _type)
{
    GLuint id = glCreateShader(_type);
    if(id == 0)
    {
        LOG_ERR("[Shader] Unable to create a shader for type %d", _type);
        return 0;
    }

    glShaderSource(id, 1, &_sources, NULL);
    glCompileShader(id);

    GLint success;
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

GLuint createShadeFromFile(const char* _path, const GLenum _type)
{
    LOG_INFO("[Shader] Create shader %s", _path);

    FILE * file;
    file = fopen(_path, "r");
    if(file == NULL)
    {
        LOG_ERR("[Shader] Unable to open the shader %s", _path);
        return 0;
    }

    fseek(file, 0L, SEEK_END);
    const long file_size = ftell(file);
    rewind(file);

    char * filebuffer = NULL;
    filebuffer = malloc(file_size * sizeof(*filebuffer));
    if (filebuffer == NULL)
    {
        LOG_ERR("[Shader] Unable to read %s (malloc %d bytes failed)", _path, filebuffer);
        return 0;
    }

    const size_t toRead = file_size - 1; // -1 because st_size includes '\0'
    size_t elementsRead = fread(filebuffer, sizeof(char), toRead, file);
    if(elementsRead != toRead)
    {
        LOG_ERR("[Shader] Unable to read %s", _path);
        return 0;
    }
    GLuint id = createShaderFromSource(filebuffer, _type);

    free(filebuffer);
    fclose(file);

    return id;
}

GLuint createShaderProgram(const GLuint _idVertexShader, const GLuint _idFragShader)
{
    GLuint id = glCreateProgram();
    if(id == 0)
    {
        LOG_ERR("[Shader] Unable to create shader program");
        return 0;
    }
    glAttachShader(id, _idVertexShader);
    glAttachShader(id, _idFragShader);

    glLinkProgram(id);
    GLint result;
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

GLuint createShaderProgramFromFile(const char* _vertexPath, const char* _fragPath)
{
    GLuint idVertex = createShadeFromFile(_vertexPath, GL_VERTEX_SHADER);
    GLuint idFrag = createShadeFromFile(_fragPath, GL_FRAGMENT_SHADER);
    return createShaderProgram(idVertex, idFrag);
}

void setShaderProgramUniform(GLuint _shaderID, const char* _name, float _x, float _y)
{
    glUseProgram(_shaderID);
    glUniform2f(glGetUniformLocation(_shaderID, _name), _x, _y);
}


