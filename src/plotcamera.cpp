#include "plotcamera.h"

PlotCamera::PlotCamera(GLuint &theProgram){

    scaleMatrix = new float[4];
    scaleMatrix[0] = 1.0;
    scaleMatrix[1] = 0.0;
    scaleMatrix[2] = 0.0;
    scaleMatrix[3] = 1.0;

    offset = new float[2];
    offset[0] = 0.0;
    offset[1] = 0.0;

    color= new float[4];
    for(int i = 0; i<4; i++){
        color[i] = 1.0;
    }
    center = new float[2];
    center[0] = 0;
    center[1] = 0;

    this->theProgram=theProgram;

    setUniforms(theProgram);
}

void PlotCamera::setUniforms(GLuint &theProgram){
    GLuint scaleUnif = glGetUniformLocation(theProgram, "scale");
    GLuint offsetUnif = glGetUniformLocation(theProgram, "offset");
    GLuint colorUnif = glGetUniformLocation(theProgram, "plotColor");

    glUseProgram(theProgram);
    glUniformMatrix2fv(scaleUnif, 1, GL_FALSE, scaleMatrix);
    glUniform2fv(offsetUnif, 1, offset);
    glUniform4fv(colorUnif, 1, color);
    glUseProgram(0);
}


float scale = 10;
void PlotCamera::resizeWindow(float w, float h){
     //nothing yet.
    float longer = w>h?w:h;
    float xscale = longer/h;
    float yscale = longer/w;
    float xmin = center[0] - scale*xscale;
    float xmax = center[0] + scale*xscale;
    float ymin = center[1] - scale*yscale;
    float ymax = center[1] + scale*yscale;

    offset[0] = -(xmax + xmin)/(xmax - xmin);
    offset[1] = -(ymax + ymin)/(ymax - ymin);
    scaleMatrix[0] = 2.0f/(xmax - xmin);
    scaleMatrix[3] = 2.0f/(ymax - ymin);

    setUniforms(theProgram);
}

void PlotCamera::trackPoint(float x, float y){
    center[0] = x;
    center[1] = y;
}
