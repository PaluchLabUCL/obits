#include "plotwindow.h"

void PlotWindow::showPlot(){
    printf("initting glfw\n");
    window = Framework::initializeWindow();

    if(window==0){
        printf("failed to load glfw\n");
        exit(1);
    }

    GLuint theProgram = Framework::loadProgram();

    GetError();

    int height, width;
    glfwGetWindowSize(window, &width, &height );

    camera = new PlotCamera(theProgram);

    camera->resizeWindow((float)width, (float)height);

    input = new PlotInteractor(camera, window);

    while( input->running==GL_TRUE )
    {


        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        input->update();

        if(setting){
            input->setCharacter(setting);
            player=setting;
            setting=0;
        }

        GetError();


        while(!queued.empty()){
            Drawable* d = queued.front();
            queued.pop();
            d->initialize(theProgram);
            plots.push_back(d);
        }
        if(player!=NULL){
            camera->trackPoint(player->getX(), player->getY());
        }
        glfwGetWindowSize(window, &width, &height );
        camera->resizeWindow((float)width, (float)height);

        for(std::list<Drawable*>::iterator pi=plots.begin(); pi!=plots.end(); pi++){
            Drawable* p = *pi;
            if(p->toRemove()){
                dying.push(p);
            } else{
                p->draw();
            }
        }

        // Swap front and back rendering buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        while(!dying.empty()){
            Drawable* dead = dying.front();
            dying.pop();
            bool removed = false;

            for(std::list<Drawable*>::iterator pi=plots.begin(); pi!=plots.end(); pi++){
                Drawable* candidate = *pi;
                if(dead==candidate){

                    plots.erase(pi);
                    removed=true;
                    break;
                }
            }

            if(removed==false){
                printf("object scheduled to be removed was not removed\n plot.cpp line 79\n");
                exit(-1);
            } else{
                delete dead;
            }
        }

    }

    // Close window and terminate GLFW
    glfwTerminate();

    // Exit program
    exit( EXIT_SUCCESS );

}

void PlotWindow::addDrawable(Drawable* d){
    queued.push(d);

}

void PlotWindow::setPlayerCharacter(Character *c){
    setting=c;
}
