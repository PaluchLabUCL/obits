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
    bool remove=false;
public:
    float vx;
    float vy;
    float x;
    float y;
    float count;
    float width;
    float height;
    float rotation[4] = {1,0,0,1};
    virtual BoundingBox* getBoundingBox();
    virtual void move();
    bool toRemove();
    void setToRemove();
    virtual void impact(float rating);
    virtual void accelerate(float value){}
    virtual void steer(float value){}
    virtual ~DynamicObject(){}

};

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
    public:
        BouncingBox(float w, float h);
        BoundingBox* getBoundingBox();
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
