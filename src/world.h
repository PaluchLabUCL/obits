#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <queue>
#include <math.h>
#include "dynamicobject.h"

class FlockBox: public DynamicObject{
    BoundingBox* box;
    DynamicObject* target;
    public:
        FlockBox(float w, float h);
        BoundingBox* getBoundingBox();
        void setTarget(DynamicObject* obj);
        void move();

    ~FlockBox(){
        delete box;
    }
};

class BouncingBox: public DynamicObject{
    BoundingBox* box;
    double timeOut = -1;
    public:
        BouncingBox(float w, float h);
        BoundingBox* getBoundingBox();
        void impact(float rating);
        void move();
        void setDuration(double t){timeOut=t;}
        ~BouncingBox(){
            delete box;
        }
};

class PlayerBox: public DynamicObject{
    BoundingBox* box;
    bool remove;
    float acc;
    float angle;
    float alpha;
    float velocity;
    public:
        PlayerBox();
        BoundingBox* getBoundingBox();
        void impact(float rating);
        void accelerate(float value);
        void steer(float value);
        void move();
        ~PlayerBox(){
            delete box;
        }
};

class DynamicsWorld{
    std::list<DynamicObject*> dynamic_objects;
    std::queue<DynamicObject*> to_remove;
    public:
        void update();
        void addDynamicObject(DynamicObject* obj);
        void removeObject(DynamicObject* obj);
        int objectCount();
};

#endif // WORLD_H
