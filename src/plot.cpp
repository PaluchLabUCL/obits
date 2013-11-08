#include "plot.h"



void PlotWindow::showPlot(){
    printf("initting glfw\n");
    GLuint theProgram = Framework::initializeWindow();

    GetError();

    int height, width;
    glfwGetWindowSize( &width, &height );
    camera = new PlotCamera(theProgram);
    
    camera->resizeWindow((float)width, (float)height);
    
    PlotInteractor* input = new PlotInteractor(camera);
    
    size_t plot_no=0;
    while( input->running==GL_TRUE )
    {
        

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        input->update();
        
        
        GetError();
        while(!queued.empty()){
            Plot* p = queued.back();
            queued.pop_back();
            p->prepareBuffers(theProgram);
            plots.push_back(p);
            plot_no = plots.size();
        }
        
        for(size_t i = 0; i<plot_no; i++){
            plots[i]->refreshBuffers(theProgram);
            camera->preparePlot(plots[i], theProgram);
            plots[i]->draw(theProgram);
        }
        
        // Swap front and back rendering buffers
        glfwSwapBuffers();

        
       
    }
    
    // Close window and terminate GLFW
    glfwTerminate();
    
    // Exit program
    exit( EXIT_SUCCESS );
    
}

void PlotWindow::addPlot(Plot* p){
        queued.push_back(p);
        
}
Plot::Plot(double* x, double* y, int length){
    dataBuffer = new double[2*length];

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(double)*2*len, dataBuffer, GL_STREAM_DRAW);
    glVertexAttribPointer(index, 2, GL_DOUBLE, GL_FALSE, 0, 0);

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
        dataBuffer=new double[2*length];
        len=length;
    }
    for(int i = 0; i<len; i++){
        dataBuffer[2*i] = x[i];
        dataBuffer[2*i+1]=y[i];
    }
    refresh=true;
}



void Plot::draw(GLuint &theProgram){

    glUseProgram(theProgram);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_STRIP, 0, len);
    GetError();
    
    glBindVertexArray(0);
    
    glUseProgram(0);
    

}

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

void PlotCamera::preparePlot(Plot* plt, GLuint &theProgram){

    for(int i = 0; i<4; i++){
        color[i] = plt->color[i];
    }

    float xfactor = 1/(plt->maxx - plt->minx);
    float yfactor = 1/(plt->maxy - plt->miny);

    scaleMatrix[0] = 2*xfactor;
    scaleMatrix[3] = 2*yfactor;

    offset[0] = -(1 + 2*xfactor*plt->minx);
    offset[1] = -(1 + 2*yfactor*plt->miny);
    setUniforms(theProgram);
}


void PlotCamera::resizeWindow(float w, float h){
 //nothing yet.   
    
}



PlotInteractor::PlotInteractor(PlotCamera* cam){
    running=GL_TRUE;
}



void PlotInteractor::update(){
    // Check if ESC key was pressed or window was closed
    running = !glfwGetKey( GLFW_KEY_ESC ) &&  glfwGetWindowParam( GLFW_OPENED );
}
