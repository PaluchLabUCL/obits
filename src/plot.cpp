#include "plot.h"



void PlotWindow::showPlot(){
    printf("initting glfw\n");
    window = Framework::initializeWindow();

    if(window==0){
        printf("failed to load glfw\n");
        exit(1);
    }

    GLuint theProgram = Framework::loadProgram();

    GetError();

    int height, width;
    glfwGetWindowSize(window, &width, &height );

    camera = new PlotCamera(theProgram);
    
    camera->resizeWindow((float)width, (float)height);
    
    input = new PlotInteractor(camera, window);
    
    size_t plot_no=0;
    while( input->running==GL_TRUE )
    {
        

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        input->update();
        
        if(setting){
            input->setCharacter(setting);
            setting=0;
        }
        
        GetError();
        while(!queued.empty()){
            Drawable* d = queued.front();
            queued.pop();
            d->initialize(theProgram);
            plots.push_back(d);
        }

        glfwGetWindowSize(window, &width, &height );
        camera->resizeWindow((float)width, (float)height);

        for(std::list<Drawable*>::iterator pi=plots.begin(); pi!=plots.end(); pi++){
            Drawable* p = *pi;
            if(p->toRemove()){
                dying.push(p);
            } else{
                p->draw();
            }
        }
        
        // Swap front and back rendering buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        while(!dying.empty()){
            Drawable* dead = dying.front();
            dying.pop();
            bool removed = false;

            for(std::list<Drawable*>::iterator pi=plots.begin(); pi!=plots.end(); pi++){
                Drawable* candidate = *pi;
                if(dead==candidate){

                    plots.erase(pi);
                    removed=true;
                    break;
                }
            }

            if(removed==false){
                printf("object scheduled to be removed was not removed\n plot.cpp line 79\n");
                exit(-1);
            } else{
                delete dead;
            }
        }

    }

    // Close window and terminate GLFW
    glfwTerminate();
    
    // Exit program
    exit( EXIT_SUCCESS );
    
}

void PlotWindow::addDrawable(Drawable* d){
        queued.push(d);
        
}

void PlotWindow::setPlayerCharacter(Character *c){
    setting=c;
}

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

Character* PlotInteractor::character = 0;
PlotInteractor::PlotInteractor(PlotCamera* cam, GLFWwindow* win){
    window = win;
    //character = (Character*)0;
    running=GL_TRUE;
}



void PlotInteractor::update(){
    running = !(glfwWindowShouldClose(window)||glfwGetKey(window, GLFW_KEY_ESCAPE));
}


void PlotInteractor::setCharacter(Character* c){
    PlotInteractor::character = c;
    glfwSetKeyCallback(window, keyCallback);
}

void PlotInteractor::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(character==0) return;
    if(key==GLFW_KEY_UP){
        character->setAcceleration(action);
    } else if(key==GLFW_KEY_DOWN){
        character->setAcceleration(-1*action);
    } else if(key==GLFW_KEY_LEFT){
        character->setSteer(1*action);
    } else if(key==GLFW_KEY_RIGHT){
        character->setSteer(-1*action);
    }
}

Character::Character(float h, float w){
    initialized=false;

    obj=0;
    color = new float[3];
    center = new float[2];
    rotation_matrix = new float[4];
    positionBuffer = new float[8];
    indexBuffer = new int[6];

    color[0] = 1;
    color[1] = 1;
    color[2] = 1;
    height = h;
    width = h;

}

void Character::setAcceleration(double direction){
    obj->accelerate(direction);
}

void Character::setSteer(double direction){
    obj->steer(direction);
}

void Character::draw(){

    if(obj!=0){
        center[0] = obj->x;
        center[1] = obj->y;

        rotation_matrix[0] = obj->rotation[0];
        rotation_matrix[1] = obj->rotation[1];
        rotation_matrix[2] = obj->rotation[2];
        rotation_matrix[3] = obj->rotation[3];

    }

    GLuint centerUniform = glGetUniformLocation(theProgram, "center");
    GLuint orientationUniform = glGetUniformLocation(theProgram, "orientation");
    GLuint colorUniform = glGetUniformLocation(theProgram, "plotColor");
    glUseProgram(theProgram);
    glUniform2fv(centerUniform, 1, center);
    glUniformMatrix2fv(orientationUniform, 1, GL_FALSE, rotation_matrix);
    glUniform3fv(colorUniform, 1, color );
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (int)6, GL_UNSIGNED_INT, 0);

    GetError();
    glBindVertexArray(0);

    glUseProgram(0);
}

void Character::initialize(GLuint &proggy){
    theProgram = proggy;
    //center = new float[2];

    center[0] = 0;
    center[1] = 0;

    //rotation_matrix = new float[4];
    for(int i = 0; i<4; i++){
        rotation_matrix[i] = IDENTITY[i];
    }
    /*
    float buff[8] = {
            -width, -height,
            -width, height,
             width, height,
             width, -height
    };
    positionBuffer = &buff[0];
    */
    //positionBuffer = new float[8];
    positionBuffer[0] = -width;
    positionBuffer[2] = -width;
    positionBuffer[4] = width;
    positionBuffer[6] = width;
    positionBuffer[1] = -height;
    positionBuffer[3] = height;
    positionBuffer[5] = height;
    positionBuffer[7] = -height;
    /*
    int ibuff[6] = {
        0, 2, 1,
        0, 3, 2
    };

    indexBuffer = &ibuff[0];
    */
    //indexBuffer = new int[6];

    indexBuffer[0] = 0; indexBuffer[1] = 2; indexBuffer[2] = 1;
    indexBuffer[3] = 0; indexBuffer[4] = 3; indexBuffer[5] = 2;



    glUseProgram(theProgram);




    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, positionBuffer, GL_STREAM_DRAW);

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6, indexBuffer, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint index = glGetAttribLocation(theProgram, "xyPosition");
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    initialized=true;
}


void Character::setDynamicObject(DynamicObject *obj){
    this->obj = obj;
}

void Character::setColor(float r, float g, float b){
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

bool Character::toRemove(){

    return obj->toRemove();
}
