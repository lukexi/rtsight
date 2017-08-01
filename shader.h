#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

char* ReadFile(const char *file);

GLuint CreateShader(GLenum ShaderType, const char* ShaderSource);

int LinkProgram(GLuint ShaderProgram);

GLuint CreateVertFragProgram(const char* VertSource, const char *FragSource);

#endif /* SHADER_H */
