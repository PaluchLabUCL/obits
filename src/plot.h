#ifndef PLOT_CLASS

#ifndef __APPLE__
    #include <GL/glew.h>
    #define DEBUG
#else
    #define GLFW_INCLUDE_GL3
    #define GLFW_NO_GLU
#endif

#include "GL/glfw.h"
#include "error.h"
#include "Framework.h"
#include <vector>
#include <pthread.h>
#include "unistd.h"



class Plot{
    double* dataBuffer;
    int len;
    GLuint positionBufferObject;
    GLuint vao;
    bool refresh;
    public:
        float minx,miny,maxx, maxy;
        float* color;
        Plot(double* x, double* y, int length);
        void prepareBuffers(GLuint &theProgram);
        void draw(GLuint &theProgram);
        void refreshBuffers(GLuint &theProgram);
        void bufferChanged(double* x, double* y, int length);
        
};

class PlotCamera{
    float* scaleMatrix;
    float* offset;
    float* color;
    void setUniforms(GLuint &theProgram);
    public:
        PlotCamera(GLuint &theProgram);
        void resizeWindow(float w, float h);
        void preparePlot(Plot* p, GLuint &theProgram);
};

class PlotInteractor{
    public:
        bool running;
        PlotInteractor(PlotCamera* camera);
        void update();

};


class PlotWindow{
    PlotCamera* camera;

    std::vector<Plot*> plots;
    std::vector<Plot*> queued;
    public:
        void showPlot();
        void addPlot(Plot* p);
};













#endif
