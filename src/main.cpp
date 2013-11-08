/*
 * For making a simple plot, and testing orbits.
 * 
 **/
#include "plot.h"
#include <stdio.h>

void actinTurnover(double* parameters, double* state, double step);
void* start_main_loop(void* ptr);

int main(int arg_no, char** args){
        
    
    
    
    pthread_t main;
    PlotWindow win;
    
    pthread_create(&main, NULL,start_main_loop,(void*)&win);
    pthread_detach(main);
    
    win.showPlot();
    
    
    
}

/**
 * @brief start_main_loop
 * @param ptr a pointer to a plot window for plotting.
 * @return nada
 */
void* start_main_loop(void* ptr){
    PlotWindow* window = (PlotWindow*)ptr;
    int N = 500;
    double* surface = new double[N];
    double* concentration = new double[N];
    double* time = new double[N];
    double tau = 1.0;
    double omega = 10.0;
    double c0 = 1.0;
    double* parameters = new double[3];
    parameters[0] = tau;
    parameters[1] = c0;
    parameters[2] = omega;

    //time c s ds_dt
    double* state = new double[4];
    state[0] = 0;
    state[1] = 1.0;
    state[2] = 1.1;
    state[3] = 0.0;

    bool first = true;
    Plot* surf = NULL;
    Plot* con = NULL;

    while(true){
        double smax = -100;
        double smin = 100;
        double cmax = -100;
        double cmin = 100;
        double step = 20.0/(N*omega);
        for(int i = 0; i<N; i++){
            time[i] = state[0];
            concentration[i] = state[1];
            surface[i] = state[2];

            if(state[2]>smax){
                smax=state[2];
            }
            if(state[2]<smin){
                smin=state[2];
            }

            if(state[1]>cmax){
                cmax=state[1];
            }
            if(state[1]<cmin){
                cmin=state[1];
            }

            actinTurnover(parameters, state, step );
        }
        if(first){
            surf = new Plot( time, surface, N );
            surf->color[1] = 0;
            surf->color[2] = 0;
            surf->maxx=time[N-1];
            surf->minx=time[0];
            surf->maxy=smax;
            surf->miny=smin;

            con = new Plot( time, concentration, N );
            con->color[0]=0;
            con->color[1]=0;
            con->maxx=time[N-1];
            con->minx=time[0];
            con->maxy=cmax;
            con->miny=cmin;
            window->addPlot(surf);

            window->addPlot(con);
            first=false;

        }else{
            surf->bufferChanged(time, surface, N);
            surf->maxx=time[N-1];
            surf->minx=time[0];
            con->bufferChanged(time,concentration, N);
            con->minx=time[0];
            con->maxx=time[N-1];
        }
        scanf("%f",&omega);
        parameters[2] = omega;


    }
    return 0;
}
/**
 * @brief actinTurnover
 * Treats actin similar to the way it is being fit, as a state variable being
 * driven by a time dependant function.
 *    //dc/dt = 1/tau c_0 - (1/tau + 1/S dS/dt)c
 * @param parameters tau, c0, omega
 * @param state c,
 * @param step
 */
const double dt = 0.00001;
void actinTurnover(double* parameters, double* state, double step){
    double time = 0;
    double ip = 1/parameters[0];
    double dS_dt, dc_dt;
    while(time<step){
        state[0] += dt;
        time += dt;
        dS_dt = state[3] + -(state[2] - 1)*parameters[2]*parameters[2]*dt;
        state[2] = state[2] + dS_dt*dt;
        state[3] = dS_dt;
        dc_dt = ip*parameters[1] - (ip + 1/state[2] * dS_dt)*state[1];
        state[1] += dc_dt*dt;
    }
}

