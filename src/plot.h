#ifndef PLOT_CLASS
#define PLOT_CLASS
#ifndef __APPLE__
    #include <GL/glew.h>
    #define DEBUG
#else
    #define GLFW_INCLUDE_GLCOREARB
    #define GLFW_NO_GLU
#endif

#include "GLFW/glfw3.h"
#include "error.h"
#include "Framework.h"
#include <list>
#include <queue>
#include <deque>
#include <pthread.h>
#include <thread>
#include "unistd.h"
#include <iostream>
#include "world.h"
#include "drawable.h"
#include "character.h"

class Plot : public Drawable{
    float* dataBuffer;
    int len;
    GLuint positionBufferObject;
    GLuint vao;
    bool refresh;
    GLuint theProgram;
    public:
        float minx,miny,maxx, maxy;
        float* color;
        Plot(double* x, double* y, int length);
        void prepareBuffers(GLuint &theProgram);
        void draw();
        void initialize(GLuint &proggy);
        void refreshBuffers(GLuint &theProgram);
        void bufferChanged(double* x, double* y, int length);
        ~Plot(){

        }
};



#endif
