#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Framework.h"
const float ORIGIN[] = {0.0f, 0.0f};
const float IDENTITY[] = {1.0f, 0.0f, 0.0f, 1.0f};

class Drawable{

public:
    virtual void draw(){}
    virtual void initialize(GLuint &proggy){}
    virtual bool toRemove(){return false;}
    virtual ~Drawable(){}
};

#endif // DRAWABLE_H
