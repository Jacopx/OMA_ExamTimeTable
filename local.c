//
// Created by munfo on 10/12/2017.
//

#include <stdlib.h>
#include <stdio.h>
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

/* Can be improved as exhaustive search
 * It try a swapping between two timeslot. If the swapped solution is better then the original it becomes the best solution
 * and the algorithm is performed again on it.
 * PROBLEM: we move on a new solution but we are stacked in a local minimum
 * TODO IMPLEMENT MAX EXECUTION TIME*/

//AGORITHM 1.1
//void localSwap(dataStructure* solution, int maxTime){
/*
    int i,j,k,E,tS;
    int *temp,*best;
    float bestBenchmark,tempBenchmark,oldBenchmark;
    E=solution->E;
    temp=malloc(E*sizeof(int));
    best=malloc(E*sizeof(int));
    // *At the start of the algorithm oldBenchmark and best benchmark are the same
    // *In this way I recall the function only if i found a new minimum
    oldBenchmark=benchmarkSolution(solution,solution->exams);
    bestBenchmark=benchmarkSolution(solution,solution->exams);
    tS=solution->timeSlots;
    for(k=0;k<E;k++)
        best[k]=solution->exams[k];

    for(i=1;i<tS-1;i++)
    {
        for (j = i + 1; j <= tS; j++)
        {
            //reset temp
            for(k=0;k<E;k++)
                temp[k]=solution->exams[k];

            for (k=0;k<E;k++)
            {
                if (temp[k] == i)
                    temp[k]=j;
                else
                if (temp[k] == j)
                    temp[k]=i;
            }

            tempBenchmark=benchmarkSolution(solution,temp);
            if(tempBenchmark<bestBenchmark)
            {
                bestBenchmark=tempBenchmark;
                for(k=0;k<E;k++)
                {
                    best[k]=temp[k];
                    solution->exams[k]=best[k];
                }

            }

        }
    }
#ifdef VERBOSE_SWAP
    printf("\nNew best Solution: benchmark:%f\nSolution->",bestBenchmark);
    for(k=0;k<E;k++)
        printf("%d ",best[k]);
    printf("\n");
#endif
    if(bestBenchmark<oldBenchmark)
        localSwap(solution,0);
    else
        localSearch(solution, 30);
}
*/

//ALGORITHM 1.2
void localSwap(dataStructure* solution, int maxTime)
{
    int i,j,k,E,tS;
    int *temp,*best;
    float bestBenchmark,tempBenchmark,oldBenchmark;
    E=solution->E;
    temp=malloc(E*sizeof(int));
    best=malloc(E*sizeof(int));
	int startTime=time(NULL);
    // *At the start of the algorithm oldBenchmark and best benchmark are the same
    // *In this way I recall the function only if i found a new minimum
    oldBenchmark=benchmarkSolution(solution,solution->exams);
    bestBenchmark=benchmarkSolution(solution,solution->exams);
    tS=solution->timeSlots;
    for(k=0;k<E;k++)
        best[k]=solution->exams[k];

    for(i=1;i<tS-1;i++)
    {
        for (j = i + 1; j <= tS; j++)
        {
            if(time(NULL)-startTime>=maxTime) return;
	        //reset temp
            for(k=0;k<E;k++)
                temp[k]=solution->exams[k];

            for (k=0;k<E;k++)
            {
                if (temp[k] == i)
                    temp[k]=j;
                else
                if (temp[k] == j)
                    temp[k]=i;
            }

            tempBenchmark=benchmarkSolution(solution,temp);
            if(tempBenchmark<bestBenchmark)
            {
                bestBenchmark=tempBenchmark;
                for(k=0;k<E;k++)
                {
                    best[k]=temp[k];
                    solution->exams[k]=best[k];
                }

            }

        }
    }
#ifdef VERBOSE_SWAP
    printf("\nNew best Solution: benchmark:%f\nSolution->",bestBenchmark);
    for(k=0;k<E;k++)
        printf("%d ",best[k]);
    printf("\n");
#endif
    //HERE THE DIFFERENCE BETWEEN 1.1 VERSION
    localSearch(solution, maxTime-(time(NULL)-startTime));
    if(bestBenchmark<oldBenchmark) {
        localSwap(solution,  maxTime-(time(NULL)-startTime));
    }
}