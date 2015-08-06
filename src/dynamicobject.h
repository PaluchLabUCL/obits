#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

const float DT=0.005;

class BoundingBox{
public:
    float x,y,w,h;
};

class DynamicObject{
    bool remove=false;
    bool finished=false;
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
    bool isFinished(){return finished;}
    void setFinished(){finished=true;}
    void setToRemove();
    virtual void impact(float rating);
    virtual void accelerate(float value){}
    virtual void steer(float value){}
    virtual ~DynamicObject(){}

};


#endif // DYNAMICOBJECT_H
