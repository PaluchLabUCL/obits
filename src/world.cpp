#include "world.h"

float ABS(float v){
    return v<0?-v:v;
}

void DynamicsWorld::addDynamicObject(DynamicObject *obj){
    dynamic_objects.push_back(obj);
}

void DynamicsWorld::update(){
    int n = dynamic_objects.size();
    for(std::list<DynamicObject*>::iterator doi = dynamic_objects.begin(); doi!=dynamic_objects.end(); doi++){
        DynamicObject* obj = *doi;

        BoundingBox* bb = obj->getBoundingBox();
        if( (bb->x<-9.0) || bb->x + bb->w>9.0 ){

           obj->vx = -obj->vx;
           obj->impact(1.0f);
           if(obj->count>10){
               to_remove.push(obj);
           }

        }

        if(bb->y < -9.0 || bb->y + bb->h>9.0){
            obj->vy = -obj->vy;
            obj->impact(1.0f);
            if(obj->count>10){
                to_remove.push(obj);
            }
        }
        std::list<DynamicObject*>::iterator odoi = doi;
        odoi++;
        for(; odoi!=dynamic_objects.end(); odoi++){
            DynamicObject* other = *odoi;
            BoundingBox* other_box = other->getBoundingBox();
            float delta_x = ABS(other_box->x - bb->x);
            if(delta_x<other_box->w||delta_x<bb->w){
                //possible collision
                float delta_y = ABS(other_box->y - bb->y);
                if(delta_y<other_box->h||delta_y<bb->h){
                    //more possible collision.
                    delta_x = ABS(other->x - obj->x);
                    delta_y = ABS(other->y - obj->y);
                    if(delta_x < other->width/2.0 && delta_y<other->height/2.0){
                        float t = obj->vx;
                        obj->vx = other->vx;
                        other->vx = t;
                        t = obj->vy;
                        obj->vy = other->vy;
                        other->vy = t;
                    }

                }
            }
        }


        obj->move();
    }
    while(!to_remove.empty()){
        DynamicObject* obj = to_remove.front();
        to_remove.pop();

        for(std::list<DynamicObject*>::iterator doi = dynamic_objects.begin(); doi!=dynamic_objects.end(); doi++){
            DynamicObject* check = *doi;
            if(check==obj){
                dynamic_objects.erase(doi);
                break;
            }
        }
        //so the graphics scene character can be removed.
        obj->setToRemove();
    }

}

int DynamicsWorld::objectCount(){
    return dynamic_objects.size();
}

void DynamicObject::impact(float rating){
    count += rating;
}

void DynamicObject::setToRemove(){

}
bool DynamicObject::toRemove(){
}

void DynamicObject::move(){
    x += vx*DT;
    y += vy*DT;
}

BoundingBox* DynamicObject::getBoundingBox(){
    return 0;
}

BouncingBox::BouncingBox(float w, float h){
    x = 0;
    y = 0;
    width = w;
    height = h;
    vx = 0.0;
    vy = 0.0;
    remove = false;
    count = 0;
    box = new BoundingBox();

}

void BouncingBox::setToRemove(){
    remove = true;
}

bool BouncingBox::toRemove(){
    return remove;
}

BoundingBox* BouncingBox::getBoundingBox(){
    float left = vx>0?0:-vx;
    float bottom = vy>0?0:-vy;
    box->x = x-width/2.0 - DT*left;
    box->y = y-height/2.0 - DT*bottom;
    box->w = width + DT*ABS(vx);
    box->h = height + DT*ABS(vy);

    return box;
}

PlayerBox::PlayerBox(){
    x = 0;
    y = 0;
    width = 0.5;
    height = 0.5;
    vx = 0.0;
    vy = 0.0;
    remove = false;
    count = 0;
    acc=0;
    angle=0;
    box = new BoundingBox();

}

BoundingBox* PlayerBox::getBoundingBox(){
    float left = vx>0?0:-vx;
    float bottom = vy>0?0:-vy;
    box->x = x-width/2.0 - DT*left;
    box->y = y-height/2.0 - DT*bottom;
    box->w = width + DT*ABS(vx);
    box->h = height + DT*ABS(vy);

    return box;
}

void PlayerBox::setToRemove(){
    remove = true;
}

bool PlayerBox::toRemove(){
    return remove;
}

void PlayerBox::impact(float rating){
    //just don't die.
}

void PlayerBox::accelerate(float value){
    acc=value;

}

void PlayerBox::move(){

    float cs = cos(angle);
    float sn = sin(angle);

    vx += cs*acc*DT;
    vy += sn*acc*DT;

    x += vx*DT;
    y += vy*DT;

}
