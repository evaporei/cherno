#include "shader.h"

#include <iostream>
#include "stdlib.h"
#include "stdio.h"

#include "glad/glad.h"

unsigned int compileShader(unsigned int kind, const char *src)
{
    unsigned int shader = glCreateShader(kind);
    const int srcLen = (const int) strlen(src);
    glShaderSource(shader, 1, &src, &srcLen);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        char message[512];
        glGetShaderInfoLog(shader, strlen(message), NULL, message);
        std::cerr << "shader go bad, err: " << message << std::endl;
    }

    return shader;
}

unsigned int createProgram(const char *vsrc, const char *fsrc)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vsrc);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fsrc);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

const char* readTextFile(const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        std::cerr << "failed to open file: " << filepath << std::endl;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *contents = (char*) malloc(fileSize + 1);

    fread(contents, 1, fileSize, file);
    contents[fileSize] = '\0';

    fclose(file);

    return (const char*) contents;
}

struct Shader shader_init(const char* vertexShaderPath, const char* fragmentShaderPath) {
    struct Shader shader;

    const char *vertexShaderSrc = readTextFile(vertexShaderPath);
    const char *fragmentShaderSrc = readTextFile(fragmentShaderPath);

    unsigned int program = createProgram(vertexShaderSrc, fragmentShaderSrc);

    glUseProgram(program);

    free((void*) vertexShaderSrc);
    free((void*) fragmentShaderSrc);
    
    shader.id = program;

    return shader;
}
