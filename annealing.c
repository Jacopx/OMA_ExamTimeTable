//
// Created by munfo on 10/12/2017.
//

#include "annealing.h"
#include "rng.h"
#include <malloc.h>
#include <stdio.h>
#include "utils.h"

#define p(delta,T) (exp(-(delta)/(T)))


const float alfa = 0.9 ;
const float T0 = 1;
const int L=100000;

void simulateAnnealingSearch(dataStructure * sol,int maxTime){
    int i,e,s,startTime;
    int *tempSol=(int *)malloc(sizeof(int)*sol->E);
    float best= benchmarkSolution(sol,sol->exams);
    float delta,T;
    for(i=0;i<sol->E;i++){
        tempSol[i]=sol->exams[i];
    }
    T=T0;
    startTime=time(NULL);
   for(i=0;time(NULL)-startTime<maxTime && T>0.001;i++){
        e=random(0,sol->E-1);
        s=random(1,sol->timeSlots);
       if(benchmarkSolution(sol,tempSol)<best){
           for(i=0;i<sol->E;i++){
               sol->exams[i]=tempSol[i];
           }
           best=benchmarkSolution(sol,sol->exams);
       }
        if(isFeasibleThis(sol,tempSol,e,s)){
            delta=benchmarkSolutionDeltaMove(sol,tempSol,e,tempSol[e],s);
            if(delta < 0 || fate(1000*p(delta,T))){
                tempSol[e]=s;
                printf("\nT:%.2f,%d->%d(%.3f,%.3f)",T,e,s,delta,benchmarkSolution(sol,tempSol));
            }
        }
       if(i%L==0) {
           T *= alfa;
       }
   }

}