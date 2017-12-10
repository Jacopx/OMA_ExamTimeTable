//
// Created by munfo on 10/12/2017.
//

#include "stdio.h"
#include "local.h"
#include "utils.h"
#include <time.h>

void localSearch(dataStructure * sol,int maxTime){
    int e,s,startTime,old;
    float minimum,val;
    struct {
        int exam;
        int slot;
        float val;
    }best;
    startTime=time(NULL);
    minimum=benchmarkSolution(sol,sol->exams);
    while(time(NULL)-startTime<maxTime){
        best.exam=-1;
        best.slot=-1;
        best.val=100000000000;
        for(e=0;e<sol->E;e++){
            for(s=1;s<=sol->timeSlots;s++){
                old=sol->exams[e];
                sol->exams[e]=s;
                if(isFeasibleThis(sol,sol->exams,e,s)){
                    val=benchmarkSolution(sol,sol->exams);
                    if(val<best.val){
                        best.exam=e;
                        best.slot=s;
                        best.val=val;
                    }
                }
                sol->exams[e]=old;
            }
        }
        if(best.val<minimum){
            sol->exams[best.exam]=best.slot;
            minimum=best.val;
            printf("\n%d->%d (%.3f)",best.exam,best.slot,best.val);
        }
        else break;
    }


}
