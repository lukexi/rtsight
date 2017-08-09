/*
SOURCES+=framebuffer.c
SOURCES+=texture.c
SOURCES+=quad.c
SOURCES+=shader.c
SOURCES+=utils.c
rtsight: main.c $(SOURCES)
    clang -o $@ $< `pkg-config --cflags --libs SDL2 GLEW` -framework OpenGL $(SOURCES)
*/

#include <SDL.h>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "texture.h"
#include "quad.h"
#include "framebuffer.h"

void DoPass(GLuint InputTexID, GLuint Program, GLuint OutputFBID) {
    glUseProgram(Program);
    glBindTexture(GL_TEXTURE_2D, InputTexID);
    glBindFramebuffer(GL_FRAMEBUFFER, OutputFBID);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

int main() {

    int ImageWidth, ImageHeight, ImageChannels;
    unsigned char *ImageData = stbi_load("computer.jpg", &ImageWidth, &ImageHeight, &ImageChannels, 0);

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* Window = SDL_CreateWindow("RTSight", 100,100, ImageWidth,ImageHeight, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(Window);
    if (glewInit() != 0) {
        goto exit;
    }

    // Create a fullscreen quad
    char* QuadVertSource = ReadFile("quad.vert");
    GLuint QuadProgram  = CreateVertFragProgram(QuadVertSource, ReadFile("quad.frag"));
    GLuint QuadVAO = CreateFullscreenQuad();



    // Load the source image
    GLuint ImageTexID = CreateTexture(ImageWidth, ImageHeight, ImageChannels);
    UpdateTexture(ImageTexID, ImageWidth, ImageHeight, GL_RGB, ImageData);


    // Create the image processing stages
    GLuint SobelProgram = CreateVertFragProgram(QuadVertSource, ReadFile("sobel.frag"));
    GLuint SobelFBTexID, SobelFBID;
    CreateFramebuffer(GL_RGB8, ImageWidth, ImageHeight, &SobelFBTexID, &SobelFBID);

    GLuint ThreshProgram = CreateVertFragProgram(QuadVertSource, ReadFile("thresh.frag"));
    GLuint ThreshFBTexID, ThreshFBID;
    CreateFramebuffer(GL_RGB8, ImageWidth, ImageHeight, &ThreshFBTexID, &ThreshFBID);

    GLuint PeaksProgram = CreateVertFragProgram(QuadVertSource, ReadFile("peaks.frag"));
    GLuint PeaksFBTexID, PeaksFBID;
    CreateFramebuffer(GL_RGB8, ImageWidth, ImageHeight, &PeaksFBTexID, &PeaksFBID);



    glClearColor(0,0,0,1);

    // Main Loop
    while (1) {
        SDL_Event Event;
        while (SDL_PollEvent(&Event)) {
            if (Event.type == SDL_QUIT) goto exit;
        }

        GLuint InputTexID = ImageTexID;

        DoPass(InputTexID, SobelProgram,  SobelFBID);  InputTexID = SobelFBTexID;
        DoPass(InputTexID, ThreshProgram, ThreshFBID); InputTexID = ThreshFBTexID;
        DoPass(InputTexID, PeaksProgram,  PeaksFBID);  InputTexID = PeaksFBTexID;

        DoPass(InputTexID, QuadProgram, 0);


        SDL_GL_SwapWindow(Window);
    }


exit:
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}
