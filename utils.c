#include "utils.h"

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include <GL/glew.h>

void Fatal(const char *format, ...)
{
    va_list ap;

    fprintf(stderr, "ERROR: ");

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    exit(1);
}

float GetTime(void)
{
    static struct timeval InitialTime = {0, 0};
    if (InitialTime.tv_sec == 0)
    {
        gettimeofday(&InitialTime, NULL);
    }
    struct timeval Now;
    gettimeofday(&Now, NULL);

    struct timeval TimeSinceStart = { Now.tv_sec - InitialTime.tv_sec, Now.tv_usec - InitialTime.tv_usec };

    return (TimeSinceStart.tv_sec + (TimeSinceStart.tv_usec / 1000000.0));
}


void PrintFps(void)
{
    static int frames = 0;
    static float startTime = -1;
    float seconds, currentTime = GetTime();

    if (startTime < 0) {
        startTime = currentTime;
    }

    frames++;

    seconds = currentTime - startTime;

    if (seconds > 5.0) {
        float fps = frames / seconds;
        printf("%d frames in %3.1f seconds = %6.3f FPS\n",
               frames, seconds, fps);
        fflush(stdout);
        startTime = currentTime;
        frames = 0;
    }
}



void GLCheck(const char* name) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("%s: ", name);
        switch (err) {
            case GL_INVALID_ENUM: printf("Invalid enum\n");
                break;
            case GL_INVALID_VALUE: printf("Invalid value\n");
                break;
            case GL_INVALID_OPERATION: printf("Invalid operation\n");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: printf("Invalid framebuffer op\n");
                break;
            case GL_OUT_OF_MEMORY: printf("GL Out of memory\n");
                break;
        }
        exit(1);
    }
}


