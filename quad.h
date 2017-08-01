#ifndef QUAD_H
#define QUAD_H

#include <GL/glew.h>


typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint UVBO;
} quad;

quad CreateQuad();
void SetQuadVertices(quad Quad, float* Vertices);

GLuint CreateFullscreenQuad();

#endif // QUAD_H
