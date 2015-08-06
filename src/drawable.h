#ifndef DRAWABLE_H
#define DRAWABLE_H

#ifndef __APPLE__
    #include <GL/glew.h>
    #define DEBUG
#else
    #define GLFW_INCLUDE_GLCOREARB
    #define GLFW_NO_GLU
#endif

#include "GLFW/glfw3.h"
#include "error.h"

class Drawable{

public:
    virtual void draw(){}
    virtual void initialize(GLuint &proggy){}
    virtual bool toRemove(){return false;}
    virtual ~Drawable(){}
};

#endif // DRAWABLE_H
