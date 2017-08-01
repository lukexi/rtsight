#include <SDL.h>
#include <GL/glew.h>
#include "shader.h"
#include "texture.h"
#include "quad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* Window = SDL_CreateWindow("RTSight", 100,100, 640,480, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(Window);
    if (glewInit() != 0) {
        goto exit;
    }

    int ImageWidth, ImageHeight, ImageChannels;
    unsigned char *ImageData = stbi_load("computer.jpg", &ImageWidth, &ImageHeight, &ImageChannels, 0);
    GLuint Texture = CreateTexture(ImageWidth, ImageHeight, ImageChannels);
    UpdateTexture(Texture, ImageWidth, ImageHeight, GL_RGB, ImageData);
    glBindTexture(GL_TEXTURE_2D, Texture);


    GLuint QuadProgram = CreateVertFragProgram(ReadFile("quad.vert"), ReadFile("quad.frag"));
    GLuint QuadVAO = CreateFullscreenQuad();

    while (1) {
        SDL_Event Event;
        while (SDL_PollEvent(&Event)) {
            if (Event.type == SDL_QUIT) goto exit;
        }

        glClearColor(0,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(QuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        SDL_GL_SwapWindow(Window);
    }


exit:
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}
