/*
 * For making a simple plot, and testing orbits.
 * 
 **/
#include "plot.h"
#include <stdio.h>

void harmonicOscillator(double* x, double* y, double pnot);
void* start_main_loop(void* ptr);
int main(int arg_no, char** args){
        
    
    
    
    pthread_t main;
    PlotWindow win;
    
    pthread_create(&main, NULL,start_main_loop,(void*)&win);
    pthread_detach(main);
    
    win.showPlot();
    
    
    
}

void* start_main_loop(void* ptr){
    PlotWindow* window = (PlotWindow*)ptr;

    double* x = new double[500];
    double* y = new double[500];

    harmonicOscillator(x,y, 1.0);


    Plot* plt = new Plot(x,y,500);
    window->addPlot(plt);

    int i = 1;

    while(true){
        double ynot = 0.0000002*(i);
        harmonicOscillator(x,y, ynot);
        plt->miny=-ynot;
        plt->maxy=ynot;

        plt->color[0] = ynot*0.5;
        plt->color[1] = ynot*0.5;
        plt->color[2] = 1-ynot*0.5;

        i++;
        if(i>=10000000){
            i=1;
        }
        plt->bufferChanged(x, y, 500);
    }
    pthread_exit(NULL);
    return 0;
}

void harmonicOscillator(double* x, double* y, double pnot){
    double v = 0;
    double pos = pnot;
    double t = -1;
    double dt=0.004;
    
    for(int i = 0; i<500; i++){
            double dv = -50*dt*pos;
            v += dv;
            pos += v*dt;
            t+=dt;
            x[i] = t;
            y[i] = pos;
            
    }
}
