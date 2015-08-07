#ifndef PLOT_CLASS
#define PLOT_CLASS

#include "Framework.h"
#include "drawable.h"
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
