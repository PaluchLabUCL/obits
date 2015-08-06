#ifndef PLOTCAMERA_H
#define PLOTCAMERA_H

#include "plot.h"

class PlotCamera{
    float* scaleMatrix;
    float* offset;
    float* center;
    float* color;
    std::vector<GLuint> programs;
    void setUniforms();
    public:
        PlotCamera();
        void addProgram(GLuint &program);
        void resizeWindow(float w, float h);
        void trackPoint(float x, float y);
};

#endif // PLOTCAMERA_H
