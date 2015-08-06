#ifndef PLOTCAMERA_H
#define PLOTCAMERA_H

#include "plot.h"

class PlotCamera{
    float* scaleMatrix;
    float* offset;
    float* center;
    float* color;
    GLuint theProgram;
    void setUniforms(GLuint &theProgram);
    public:
        PlotCamera(GLuint &theProgram);
        void resizeWindow(float w, float h);
        void trackPoint(float x, float y);
};

#endif // PLOTCAMERA_H
