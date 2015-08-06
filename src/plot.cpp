#include "plot.h"




Plot::Plot(double* x, double* y, int length){
    dataBuffer = new float[2*length];

    for(int i = 0; i<length; i++){
        dataBuffer[2*i] = x[i];
        dataBuffer[2*i+1]=y[i];
    }
    minx=-1;
    miny=-1;
    maxx=1;
    maxy=1;

    color=new float[4];

    for(int i = 0;i<4;i++){
        color[i] = 1.0f;
    }

    len=length;
    refresh=false;
}

void Plot::prepareBuffers(GLuint &theProgram){
    
    glUseProgram(theProgram);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint index = glGetAttribLocation(theProgram, "xyPosition");

    glGenBuffers(1, &positionBufferObject);
    
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*len, dataBuffer, GL_STREAM_DRAW);
    glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GetError();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    
    GetError();

}

void Plot::refreshBuffers(GLuint &theProgram){
    if(!refresh) return;
    glUseProgram(theProgram);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(double)*len*2, dataBuffer);

    GetError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

}

void Plot::bufferChanged(double* x, double* y, int length){
    if(len!=length){
        delete [] dataBuffer;
        dataBuffer=new float[2*length];
        len=length;
    }
    for(int i = 0; i<len; i++){
        dataBuffer[2*i] = x[i];
        dataBuffer[2*i+1]=y[i];
    }
    refresh=true;
}



void Plot::draw(){

    GLuint center = glGetUniformLocation(theProgram, "center");
    GLuint orientation = glGetUniformLocation(theProgram, "orientation");

    glUseProgram(theProgram);
    glUniform2fv(center, 1, ORIGIN);
    glUniformMatrix2fv(orientation, 1, GL_FALSE, IDENTITY);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_STRIP, 0, len);
    GetError();
    glBindVertexArray(0);
    
    glUseProgram(0);
    

}

void Plot::initialize(GLuint &proggy){
    theProgram = proggy;
    prepareBuffers(theProgram);
}
