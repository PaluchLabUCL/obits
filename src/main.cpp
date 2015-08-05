/*
 * For making a simple plot, and testing orbits.
 * 
 **/
#include "plot.h"
#include <stdio.h>
#include "world.h"
#include <math.h>

void* start_main_loop(void* ptr);
void generateFlockObjects(PlotWindow* win, DynamicsWorld* world, DynamicObject* target);
void generateExplosionObject(PlotWindow* win, DynamicsWorld* world, DynamicObject* target);
double randomNG();
int main(int arg_no, char** args){
        
    
    
    
    pthread_t main;
    PlotWindow win;
    
    pthread_create(&main, NULL,start_main_loop,(void*)&win);
    pthread_detach(main);
    
    win.showPlot();
    
    
    
}

/**
 * @brief start_main_loop
 * @param ptr a pointer to a plot window for plotting.
 * @return nada
 */
void* start_main_loop(void* ptr){
    PlotWindow* window = (PlotWindow*)ptr;
    double* x = new double[5];
    double* y = new double[5];
    x[0] = -9.0;
    y[0] = -9.0;
    x[1] = -9.0;
    y[1] = 9.0;
    x[2] = 9.0;
    y[2] = 9.0;
    x[3] = 9.0;
    y[3] = -9.0;
    x[4] = -9.0;
    y[4] = -9.0;
    Plot* plot = new Plot(x,y,5);
    window->addDrawable(plot);


    DynamicsWorld* world = new DynamicsWorld();
    DynamicObject* obj = new PlayerBox();
    generateFlockObjects(window, world, obj);

    //time_t start, finished;
    double left;


    world->addDynamicObject(obj);
    obj->vx = 0;
    obj->vy = 0;
    obj->x = 0;
    obj->y = 0;
    Character* c = new Character(0.5f, 0.5f);
    c->setColor(1.0f, 0.0f, 0.0f);
    c->setDynamicObject(obj);
    window->addDrawable(c);
    window->setPlayerCharacter(c);
    while(true){
        glfwSetTime(0.0);
        world->update();

        float c = obj->count;
        if(c>0){
            generateExplosionObject(window, world, obj);
            obj->impact(-c);
        }


        double left = 0.0166 - glfwGetTime();
        if(left>0){
            usleep((int)(left*10000));
        }
    }

    return 0;
}

void generateFlockObjects(PlotWindow* win, DynamicsWorld* world, DynamicObject* target){
    for(int i = 0; i<50; i++){

        float w = 0.02;
        float h = 0.02;

        FlockBox* obj = new FlockBox(w,h);
        obj->setTarget(target);
        world->addDynamicObject(obj);

        obj->vx = cos(i*3.14*0.004);
        obj->vy = sin(i*3.14*0.004);


        obj->vx = cos(i*3.14*0.004);
        obj->vy = sin(i*3.14*0.004);

        Character* c = new Character(w, h);
        c->setColor((i%51)/50.0f, (i%25)/20.0f, 1.0f);
        c->setDynamicObject(obj);
        win->addDrawable(c);
    }
}

void generateExplosionObject(PlotWindow* win, DynamicsWorld* world, DynamicObject* target){
    for(int i = 0; i<100; i++){

        float w = 0.02;
        float h = 0.02;

        BouncingBox* obj = new BouncingBox(w,h);

        world->addDynamicObject(obj);

        obj->vx = cos(i*3.14*0.02);
        obj->vy = sin(i*3.14*0.02);


        obj->x = target->x;
        obj->y = target->y;

        Character* c = new Character(w, h);
        c->setColor((i%51)/50.0f, (i%25)/20.0f, 1.0f);
        c->setDynamicObject(obj);
        win->addDrawable(c);

    }
}

const double factor = 1.0/RAND_MAX;
double randomNG(){

    return std::rand()*factor;
}
