//
// Created by munfo on 10/12/2017.
//

#include "stdio.h"
#include "local.h"
#include <time.h>

const float minimiumDelta = 0.00001;


void localSearch(dataStructure *sol, int maxTime) {
    int e,s,startTime;
    float delta;
    struct {
        int exam;
        int slot;
        float delta;
    }best;
    startTime=time(NULL);
    while(time(NULL)-startTime<maxTime){
        best.exam=-1;
        best.slot=-1;
        best.delta=100000000000;
        for(e=0;e<sol->E;e++){
            for(s=1;s<=sol->timeSlots;s++){
                if(isFeasibleThis(sol,sol->exams,e,s)){
                    delta=benchmarkSolutionDeltaMove(sol,sol->exams,e,sol->exams[e],s);
                    if(delta<best.delta){
                        best.exam=e;
                        best.slot=s;
                        best.delta=delta;
                    }
                }
            }
        }
        if(best.delta<-minimiumDelta){
            sol->exams[best.exam]=best.slot;
#ifdef VERBOSE_LOCAL
            printf("\n%d->%d (%.3f,%.3f)",best.exam,best.slot,best.delta,benchmarkSolution(sol,sol->exams));
#endif
        }
        else break;
    }
}
