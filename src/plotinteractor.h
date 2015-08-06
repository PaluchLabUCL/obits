#ifndef PLOTINTERACTOR_H
#define PLOTINTERACTOR_H
#include "plot.h"
#include "plotcamera.h"
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



#endif // PLOTINTERACTOR_H
