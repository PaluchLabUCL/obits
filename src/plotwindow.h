#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "plot.h"
#include "plotcamera.h"
#include "plotinteractor.h"

class PlotWindow{
    PlotCamera* camera;
    GLFWwindow* window;
    Character* setting = NULL;
    Character* player = NULL;
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



#endif // PLOTWINDOW_H
