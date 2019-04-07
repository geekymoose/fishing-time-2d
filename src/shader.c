#include "shader.h"

#include <sys/stat.h>
#include <stdio.h>

#include "log.h"


GLuint createShaderFromSource(const char* _sources, const GLenum _type)
{
    GLuint id = glCreateShader(_type);
    if(id == 0)
    {
        LOG_ERR("Unable to create a shader for type %d\n", _type);
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
        LOG_ERR("Unable to compile shader %d: %s\n", _type, strlog);
        return 0;
    }

    return id;
}

GLuint createShadeFromFile(const char* _path, const GLenum _type)
{
    FILE * file;
    file = fopen(_path, "r");
    if(file == NULL)
    {
        LOG_ERR("Unable to open the shader file at path %s\n", _path);
        return 0;
    }

    struct stat filestat;
    stat(_path, &filestat);
    char filebuffer[filestat.st_size];
    const size_t toRead = filestat.st_size - 1; // -1 cuz st_size includes '\0'
    size_t elementsRead = fread(filebuffer, sizeof(char), toRead, file);
    if(elementsRead != toRead)
    {
        LOG_ERR("Unable to read from %s\n", _path);
        return 0;
    }
    GLuint id = createShaderFromSource(filebuffer, _type);

    fclose(file);
    return id;
}

GLuint createShaderProgram(const GLuint _idVertexShader, const GLuint _idFragShader)
{
    GLuint id = glCreateProgram();
    if(id == 0)
    {
        LOG_ERR("Unable to create shader program\n");
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
        LOG_ERR("Unable to link shader program %s\n", loginfo);
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


