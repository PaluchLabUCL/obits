#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <queue>
#include <math.h>

const float DT=0.005;

class BoundingBox{
public:
    float x,y,w,h;
};

class DynamicObject{

public:
    float vx;
    float vy;
    float x;
    float y;
    float count;
    float width;
    float height;
    virtual BoundingBox* getBoundingBox();
    virtual void move();
    virtual bool toRemove();
    virtual void setToRemove();
    virtual void impact(float rating);
    virtual void accelerate(float value){}
    virtual ~DynamicObject(){}

};

class BouncingBox: public DynamicObject{
    BoundingBox* box;
    bool remove;
    public:
        BouncingBox(float w, float h);
        BoundingBox* getBoundingBox();
        //void move();
        bool toRemove();
        void setToRemove();
        //virtual void impact(float rating);
        ~BouncingBox(){
            delete box;
        }
};

class PlayerBox: public DynamicObject{
    BoundingBox* box;
    bool remove;
    float acc;
    float angle;

    public:
        PlayerBox();
        BoundingBox* getBoundingBox();
        bool toRemove();
        void setToRemove();
        void impact(float rating);
        void accelerate(float value);
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
