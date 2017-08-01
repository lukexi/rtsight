#include "quad.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>

static float FullscreenQuadVertices[8] = {
    -1, -1, // Left Top
    -1, 1,  // Left Bottom
    1, -1,  // Right Top
    1, 1    // Right Bottom
};


static float FullscreenQuadUVs[8] = {
    0, 1, // Left Top
    0, 0, // Left Bottom
    1, 1, // Right Top
    1, 0  // Right Bottom
};

quad CreateQuad() {

    GLuint VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);


    /*

    1__3
    | /|
    |/_|
    2  4

    */
    const int NumVertComponents = 2;
    float Vertices[8] = {0};

    GLuint VertBuffer;
    glGenBuffers(1, &VertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        0,                  // Attribute Index
        NumVertComponents,  // Attribute Size
        GL_FLOAT,           // Attribute Type
        GL_FALSE,           // Normalize values?
        0,                  // Stride
        0                   // Pointer to first item
        );
    glEnableVertexAttribArray(0);


    // UVQs
    const int NumUVComponents = 3;
    float UVs[12] = {0};

    GLuint UVsBuffer;
    glGenBuffers(1, &UVsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, UVsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UVs), UVs, GL_DYNAMIC_DRAW);


    glVertexAttribPointer(
        1,                  // Attribute Index
        NumUVComponents,    // Attribute Size
        GL_FLOAT,           // Attribute Type
        GL_FALSE,           // Normalize values?
        0,                  // Stride
        0                   // Pointer to first item
        );
    glEnableVertexAttribArray(1);

    GLCheck("Quad construction");

    return (quad){.VAO=VAO, .VBO=VertBuffer, .UVBO=UVsBuffer};
}

float Distance(float X1, float Y1, float X2, float Y2) {
    return sqrtf(powf(X2-X1,2) + powf(Y2-Y1, 2));
}

void SetQuadVertices(quad Quad, float* Vertices) {

    // Write the new vertices
    glBindBuffer(GL_ARRAY_BUFFER, Quad.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), Vertices);

    // Recalculate Quadrilateral-interpolation UVQs
    float X1 = Vertices[0]; float Y1 = Vertices[1];
    float X2 = Vertices[2]; float Y2 = Vertices[3];
    float X3 = Vertices[4]; float Y3 = Vertices[5];
    float X4 = Vertices[6]; float Y4 = Vertices[7];

    // Find line-line intersection point
    float PX = ((X1*Y4-Y1*X4)*(X2-X3) - (X1-X4)*(X2*Y3-Y2*X3))
             / ((X1-X4)*(Y2-Y3) - (Y1-Y4)*(X2-X3));
    float PY = ((X1*Y4-Y1*X4)*(Y2-Y3) - (Y1-Y4)*(X2*Y3-Y2*X3))
             / ((X1-X4)*(Y2-Y3) - (Y1-Y4)*(X2-X3));

    // Calculate distances to intersection point
    float D1 = Distance(X1,Y1,PX,PY);
    float D2 = Distance(X2,Y2,PX,PY);
    float D3 = Distance(X3,Y3,PX,PY);
    float D4 = Distance(X4,Y4,PX,PY);

    // Calculate UVQ
    // http://www.reedbeta.com/blog/quadrilateral-interpolation-part-1/
    //
    // (Coefficients calculated by distance
    // Dn = Distance from Corner to center
    // Do = Distance from diagnonal-opposite corner to center
    // in (Dn+Do)/Do
    // So in vert layout those matchings are:
    // 1->4, 2->3
    float C1 = (D1 + D4) / D4;
    float C2 = (D2 + D3) / D3;
    float C3 = (D3 + D2) / D2;
    float C4 = (D4 + D1) / D1;
    float UVQs[12] = {
        0*C1, 0*C1, 1*C1,
        0*C2, 1*C2, 1*C2,
        1*C3, 0*C3, 1*C3,
        1*C4, 1*C4, 1*C4,
    };

    // Write the new UVQs
    glBindBuffer(GL_ARRAY_BUFFER, Quad.UVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(UVQs), UVQs);
}

// Simpler quad that uses regular static vertices and V2 UVs
GLuint CreateFullscreenQuad() {

    GLuint VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);


    /*

    1__3
    | /|
    |/_|
    2  4

    */
    const int NumVertComponents = 2;

    GLuint VertBuffer;
    glGenBuffers(1, &VertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(FullscreenQuadVertices), FullscreenQuadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // Attribute Index
        NumVertComponents,  // Attribute Size
        GL_FLOAT,           // Attribute Type
        GL_FALSE,           // Normalize values?
        0,                  // Stride
        0                   // Pointer to first item
        );
    glEnableVertexAttribArray(0);


    // UVQs
    const int NumUVComponents = 2;

    GLuint UVsBuffer;
    glGenBuffers(1, &UVsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, UVsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(FullscreenQuadUVs), FullscreenQuadUVs, GL_STATIC_DRAW);

    glVertexAttribPointer(
        1,                  // Attribute Index
        NumUVComponents,    // Attribute Size
        GL_FLOAT,           // Attribute Type
        GL_FALSE,           // Normalize values?
        0,                  // Stride
        0                   // Pointer to first item
        );
    glEnableVertexAttribArray(1);

    GLCheck("Quad construction");

    return VAO;
}
