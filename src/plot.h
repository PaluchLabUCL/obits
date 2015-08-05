#ifndef PLOT_CLASS

#ifndef __APPLE__
    #include <GL/glew.h>
    #define DEBUG
#else
    #define GLFW_INCLUDE_GLCOREARB
    #define GLFW_NO_GLU
#endif

#include "GLFW/glfw3.h"
#include "error.h"
#include "Framework.h"
#include <list>
#include <queue>
#include <deque>
#include <pthread.h>
#include <thread>
#include "unistd.h"
#include <iostream>
#include "world.h"

const float ORIGIN[] = {0.0f, 0.0f};
const float IDENTITY[] = {1.0f, 0.0f, 0.0f, 1.0f};

class Drawable{

public:
    virtual void draw(){}
    virtual void initialize(GLuint &proggy){}
    virtual bool toRemove(){return false;}
    virtual ~Drawable(){}
};

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
        void setAcceleration(double direction);
        void setSteer(double direction);
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
};

class PlotCamera{
    float* scaleMatrix;
    float* offset;
    float* center;
    float* color;
    GLuint theProgram;
    void setUniforms(GLuint &theProgram);
    public:
        PlotCamera(GLuint &theProgram);
        void resizeWindow(float w, float h);
};

class PlotInteractor{
    GLFWwindow* window;
    static Character* character;
    public:
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        PlotInteractor(PlotCamera* camera, GLFWwindow* win);
        bool running;
        void update();
        void setCharacter(Character* c);

};


class PlotWindow{
    PlotCamera* camera;
    GLFWwindow* window;
    Character* setting;
    PlotInteractor* input;
    std::list<Drawable*> plots;
    std::queue<Drawable*> queued;
    std::queue<Drawable*> dying;
    std::mutex mutex;
    public:
        void showPlot();
        void addDrawable(Drawable* d);
        void setPlayerCharacter(Character* c);
};













#endif
