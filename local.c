/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include "local.h"
#include "utils.h"
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
	localSearch2Temp(sol,sol->exams,maxTime-(time(NULL)-startTime));
	localSearchSlideTemp(sol,sol->exams,maxTime-(time(NULL)-startTime));
}
void localSearchTemp(dataStructure *sol,int* temp, int maxTime) {
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
                if(isFeasibleThis(sol,temp,e,s)){
                    delta=benchmarkSolutionDeltaMove(sol,temp,e,temp[e],s);
                    if(delta<best.delta){
                        best.exam=e;
                        best.slot=s;
                        best.delta=delta;
                    }
                }
            }
        }
        if(best.delta<-minimiumDelta){
            temp[best.exam]=best.slot;
#ifdef VERBOSE_LOCAL
            printf("\nT%d->%d (%.3f,%.3f)",best.exam,best.slot,best.delta,benchmarkSolution(sol,temp));
#endif
        }
        else break;
    }
	float old=benchmarkSolution(sol,temp);
	localSearch2Temp(sol,temp,maxTime-(time(NULL)-startTime));
	localSearchSlideTemp(sol,temp,maxTime-(time(NULL)-startTime));
	if(old-benchmarkSolution(sol,temp)>minimiumDelta){
		localSearchTemp(sol,temp,time(NULL)-startTime);
	}
}
void swapExam(int *sol,int a,int b){
    int temp=sol[a];
    sol[a]=sol[b];
    sol[b]=temp;
}
void slideSlot(dataStructure* sol,int *temp,int from,int to, int pos) {
	for(int i=0;i<sol->E;i++){
		if(temp[i]>=from && temp[i]<=to){
			temp[i] = temp[i] + pos;
			if(temp[i]>to) temp[i]=from+temp[i]-to-1;
		}

	}
}

void localSearch2Temp(dataStructure *sol,int* temp, int maxTime) {
    int e1,e2,startTime;
    float cost;
    struct {
        int exam1;
        int exam2;
        float cost;
    }best;
    startTime=time(NULL);
    while(time(NULL)-startTime<maxTime){
        best.exam1=-1;
        best.exam2=-1;
        best.cost=100000000000;
        for(e1=0;e1<sol->E;e1++){
            for(e2=0;e2<e1;e2++) {
                if (isFeasibleSwap(sol, temp,e1,e2)) {
	                cost=benchmarkSolutionSwap(sol,temp,e1,e2);
                    if (cost < best.cost) {
                        best.exam1 = e1;
                        best.exam2 = e2;
                        best.cost = cost;
                    }
                }
            }
        }
        if(best.cost<-minimiumDelta){
            swapExam(temp,best.exam1,best.exam2);
#ifdef VERBOSE_LOCAL
            printf("\nT%d<->%d (%.3f,%.3f)",best.exam1,best.exam2,best.cost,benchmarkSolution(sol,temp));
#endif
        }
        else break;
    }
}
void localSearchSlideTemp(dataStructure *sol,int* temp, int maxTime) {
    int from,to,startTime;
	int *t =malloc(sizeof(int)*sol->E);
    float cost;
    struct {
        int from;
        int to;
	    int shift;
        float cost;
    }best;
    startTime=time(NULL);
    while(time(NULL)-startTime<maxTime){
        best.from=-1;
        best.to =-1;
	    best.shift=-1;
        best.cost=100000000000;
        for(from=1;from<sol->timeSlots;from++){
            for(to=from+1;to<=sol->timeSlots;to++) {
	            for (int shift = 1; shift < to - from; shift++) {
		            copyArray(t, temp, sol->E);
		            slideSlot(sol, t, from, to, shift);
		            if (isFeasible(sol, t)) {
			            cost = benchmarkSolution(sol, t);
			            if (cost < best.cost) {
				            best.from = from;
				            best.to = to;
				            best.shift=shift;
				            best.cost = cost;
			            }
		            }
	            }
            }
        }
        if(benchmarkSolution(sol,temp)-best.cost>minimiumDelta){
            slideSlot(sol,temp,best.from,best.to,best.shift);
#ifdef VERBOSE_LOCAL
            printf("\nT%d-%d>>%d (%.3f,%.3f)",best.from,best.to,best.shift,best.cost,benchmarkSolution(sol,temp));
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
void localSwap(dataStructure* solution, int maxTime) {
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
                }
                #ifdef VERBOSE_SWAP
                    printf("\nNew best Solution: benchmark:%f\nSolution->",bestBenchmark);
                    for(k=0;k<E;k++)
                        printf("%d ",best[k]);
                    printf("\n");
                #endif
            }

        }
    }

    for(k=0;k<E;k++)
    {
        solution->exams[k]=best[k];
    }
    if(bestBenchmark<oldBenchmark)
        localSwap(solution, maxTime-(time(NULL)-startTime));
    else
        localSearch(solution, maxTime-(time(NULL)-startTime));
}

void localSwapTemp(dataStructure* solution,int *tempSol, int maxTime) {
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
        best[k]=tempSol[k];

    for(i=1;i<tS-1;i++)
    {
        for (j = i + 1; j <= tS; j++)
        {
            if(time(NULL)-startTime>=maxTime) return;
            //reset temp
            for(k=0;k<E;k++)
                temp[k]=tempSol[k];

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
                }
                #ifdef VERBOSE_SWAP
                    printf("\nTempNew best Solution: benchmark:%f\nSolution->",bestBenchmark);
                    for(k=0;k<E;k++)
                        printf("%d ",best[k]);
                    printf("\n");
                #endif
            }

        }
    }

    for(k=0;k<E;k++)
    {
        tempSol[k]=best[k];
    }
    if(bestBenchmark<oldBenchmark)
        localSwapTemp(solution,tempSol, maxTime-(time(NULL)-startTime));
    else
        localSearchTemp(solution,tempSol,maxTime-(time(NULL)-startTime));
}


//ALGORITHM 1.2
/*void localSwap(dataStructure* solution, int maxTime)
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
    bestBenchmark=oldBenchmark;
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
                    //solution->exams[k]=best[k];
                }
                #ifdef VERBOSE_SWAP
                                printf("\nNew best Solution: benchmark:%f\nSolution->",bestBenchmark);
                                for(k=0;k<E;k++)
                                    printf("%d ",best[k]);
                                printf("\n");
                #endif
            }

        }
    }
    for(k=0;k<E;k++)
    {
        solution->exams[k]=best[k];
    }
    //HERE THE DIFFERENCE BETWEEN 1.1 VERSION
    localSearch(solution, maxTime-(time(NULL)-startTime));
    if(bestBenchmark<oldBenchmark) {
        localSwap(solution,  maxTime-(time(NULL)-startTime));
    }
}*/



