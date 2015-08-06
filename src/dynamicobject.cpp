#include "dynamicobject.h"

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
