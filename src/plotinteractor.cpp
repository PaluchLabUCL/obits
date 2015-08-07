#include "plotinteractor.h"

Character* PlotInteractor::character = 0;

PlotInteractor::PlotInteractor(PlotCamera* cam, GLFWwindow* win){
    window = win;
    running=GL_TRUE;

}



void PlotInteractor::update(){
    running = !(glfwWindowShouldClose(window)||glfwGetKey(window, GLFW_KEY_ESCAPE));
}


void PlotInteractor::setCharacter(Character* c){
    PlotInteractor::character = c;
    glfwSetKeyCallback(window, keyCallback);
}

void PlotInteractor::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(character==0) return;
    if(key==GLFW_KEY_UP){
        character->setAcceleration(action);
    } else if(key==GLFW_KEY_DOWN){
        character->setAcceleration(-1*action);
    } else if(key==GLFW_KEY_LEFT){
        character->setSteer(1*action);
    } else if(key==GLFW_KEY_RIGHT){
        character->setSteer(-1*action);
    } else if(key==GLFW_KEY_SPACE){
        character->spaceAction();
    }
}
