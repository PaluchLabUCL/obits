#ifndef CHARACTER_H
#define CHARACTER_H

#include "drawable.h"
#include "dynamicobject.h"
#include <iostream>


class Character : public Drawable{
    float* positionBuffer;
    int* indexBuffer;
    GLuint positionBufferObject;
    GLuint indexBufferObject;
    GLuint vao;
    GLuint theProgram;
    float* center;
    float* rotation_matrix;
    float* color;
    DynamicObject* obj;
    float height, width;
    bool initialized;
    bool deleted=false;
    Character();
    public:
        Character(float w, float h);
        void prepareBuffers(GLuint &proggy);
        void draw();
        void initialize(GLuint &proggy);
        void move(float dx, float dy);
        void setDynamicObject(DynamicObject* obj);
        void setColor(float r, float g, float c);
        bool toRemove();
        float getX(){return center[0];}
        float getY(){return center[1];}
        void setAcceleration(double direction);
        void setSteer(double direction);
        void spaceAction();
        ~Character(){shutdown();}
        void shutdown(){
            if(deleted) exit(-1); else deleted=true;
            delete[] positionBuffer;
            delete[] indexBuffer;
            glDeleteBuffers(1, &positionBufferObject);
            glDeleteBuffers(1, &indexBufferObject);
            glDeleteVertexArrays(1, &vao);
            delete[] center;
            delete[] rotation_matrix;
            delete[] color;
            delete obj;
        }

};


#endif // CHARACTER_H
