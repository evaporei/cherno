#include "shader.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "glad/glad.h"

unsigned int compileShader(unsigned int kind, const char *src) {
    unsigned int shader = glCreateShader(kind);
    const int srcLen = (const int) strlen(src);
    glShaderSource(shader, 1, &src, &srcLen);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        char message[512];
        glGetShaderInfoLog(shader, strlen(message), NULL, message);
        fprintf(stderr, "shader go bad, err: %s\n", message);
    }

    return shader;
}

unsigned int createProgram(const char *vsrc, const char *fsrc) {
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

const char* readTextFile(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "failed to open file: %s\n", filepath);
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

void shader_bind(struct Shader self) {
    glUseProgram(self.id);
}

int get_uniform_location(struct Shader *self, const char *name) {
    if (self->uniformLocationCache.contains(name))
        return self->uniformLocationCache[name];
    int location = glGetUniformLocation(self->id, name);
    if (location == -1)
        printf("not using uniform %s\n", name);
    self->uniformLocationCache[name] = location;
    return location;
}

void shader_set_uniform_4f(struct Shader *self, const char *name, float v0, float v1, float v2, float v3) {

    glUniform4f(get_uniform_location(self, name), v0, v1, v2, v3);
}

void shader_set_uniform_1i(struct Shader *self, const char* name, int v) {

    glUniform1i(get_uniform_location(self, name), v);
}

void shader_set_uniform_mat4f(struct Shader *self, const char *name, glm::mat4 m) {
    glUniformMatrix4fv(get_uniform_location(self, name), 1, GL_FALSE, &m[0][0]);
}
