#include "character.h"

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
    positionBuffer[2] = -0.9*width;
    positionBuffer[4] = 0.9*width;
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

void Character::spaceAction(){
    obj->impact(1);
}
