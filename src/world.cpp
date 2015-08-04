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
               //to_remove.push(obj);
           }

        }

        if(bb->y < -9.0 || bb->y + bb->h>9.0){
            obj->vy = -obj->vy;
            obj->impact(1.0f);
            if(obj->count>10){
                //to_remove.push(obj);
            }
        }
        /*
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

        }*/


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
    remove = true;
}
bool DynamicObject::toRemove(){
	return remove;
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
    count = 0;
    box = new BoundingBox();

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

FlockBox::FlockBox(float w, float h){
    x = 0;
    y = 0;
    width = w;
    height = h;
    vx = 0.0;
    vy = 0.0;
    count = 0;
    box = new BoundingBox();

}

void FlockBox::move(){
    x += vx*DT;
    y += vy*DT;


    BoundingBox* obox = target->getBoundingBox();
    double ox = obox->x + 0.5*obox->w;
    double oy = obox->y + 0.5*obox->h;

    double dx = ox - x;
    double dy = oy -y;
    double m = dx*dx + dy*dy;
    if(m > 4){

        vx = 0.5*(vx + dx);
        vy = 0.5*(vy + dy);

    }

}



BoundingBox* FlockBox::getBoundingBox(){
    float left = vx>0?0:-vx;
    float bottom = vy>0?0:-vy;
    box->x = x-width/2.0 - DT*left;
    box->y = y-height/2.0 - DT*bottom;
    box->w = width + DT*ABS(vx);
    box->h = height + DT*ABS(vy);

    return box;
}

void FlockBox::setTarget(DynamicObject *obj){
    target=obj;
}


PlayerBox::PlayerBox(){
    x = 0;
    y = 0;
    width = 0.5;
    height = 0.5;
    vx = 0.0;
    vy = 0.0;
    velocity=0;
    remove = false;
    count = 0;
    acc=0;
    angle=0;
    alpha=0;
    rotation[0] = 1;
    rotation[1] = 0;
    rotation[2] = 0;
    rotation[3] = 1;
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

void PlayerBox::impact(float rating){
    //just don't die.
}

void PlayerBox::accelerate(float value){
    acc=value;
}

void PlayerBox::steer(float value){
    alpha=value;
}

void PlayerBox::move(){
    velocity = sqrt(vx*vx + vy*vy);

    if(velocity>0){
        rotation[0] = vy/velocity;
        rotation[1] = -vx/velocity;
        rotation[2] = vx/velocity;
        rotation[3] = vy/velocity;
    }

    if(alpha!=0){
        float a = cos(-alpha*0.0005);
        float b = sin(-alpha*0.0005);
        float c = rotation[0]*a - rotation[2]*b;
        float s = rotation[0]*b + rotation[2]*a;

        float mag = 1.0/sqrt(c*c + s*s);


        rotation[0] = c*mag;
        rotation[2] = s*mag;

        rotation[1] = -rotation[2];
        rotation[3] = rotation[0];

    }

    velocity = 0.995*velocity + acc*DT;

    vx = rotation[2]*velocity;
    vy = rotation[0]*velocity;

    x += vx*DT;
    y += vy*DT;

}
