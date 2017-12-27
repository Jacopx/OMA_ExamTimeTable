/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "annealing.h"
#include "rng.h"
#include "local.h"
#include "utils.h"
#include <stdio.h>

#define p(delta,T) (exp(-(delta)/(T)))

const float alfa = 0.9;
const float T0 = 1000;
const int L=100;

void simulateAnnealingSearch(dataStructure * sol,int turn,int maxTime){
    int i,j,e,s,startTime,flag=0 ;
    TempSol *tempSol=newTempSol(sol);
    float best= benchmarkSolution(sol,sol->exams);
    float delta,T;
    for(j=0;j<sol->E;j++){
        tempSol->temporarySolution[j]=sol->exams[j];
    }

    startTime=time(NULL);
	T=T0;
	for (i = 0; T > 0.001 && (time(NULL)-startTime)<maxTime;) {
		e = random(0, sol->E - 1);
		s = random(1, sol->timeSlots);
		if (benchmarkSolution(sol, tempSol->temporarySolution) < best && isFeasible(sol,tempSol->temporarySolution)) {
			for (i = 0; i < sol->E; i++) {
				sol->exams[i] = tempSol->temporarySolution[i];
			}
			best = benchmarkSolution(sol, sol->exams);
		}
		if (flag<turn || isFeasibleThis(sol, tempSol->temporarySolution, e, s)) {
			i++;
			delta = benchmarkSolutionDeltaMove(sol, tempSol->temporarySolution, e, tempSol->temporarySolution[e],
			                                   s);
			if (delta < 0 || fate(10000 * p(delta, T))) {
				if(!isFeasibleThis(sol,tempSol->temporarySolution,e,s)) flag++;
				tempSol->temporarySolution[e] = s;
				//if(delta>0) T*=alfa;
#ifdef VERBOSE_ANNEALING
				printf("\nConf:%d/%d T:%.4f,%d->%d(%.3f,%.3f,%.3f)",flag,turn, T, e, s, delta,
					   benchmarkSolution(sol, tempSol->temporarySolution), best);
#endif
			}
		}
		if (i % L == 0) {
			T *= alfa;
		}
	}
	extendSol(sol,tempSol);
	findFeasibleSolution(sol,tempSol,maxTime-(time(NULL)-startTime));
	if(arrayEquals(tempSol->temporarySolution,sol->exams,sol->E)) return;
	localSearchTemp(sol,tempSol->temporarySolution,maxTime-(time(NULL)-startTime));
	if (benchmarkSolution(sol, tempSol->temporarySolution) < best && isFeasible(sol,tempSol->temporarySolution)) {
		for (i = 0; i < sol->E; i++) {
			sol->exams[i] = tempSol->temporarySolution[i];
		}
	}
}

void simulateAnnealingSearch2(dataStructure * sol,int turn,int maxTime){
    int i,j,e1,e2,startTime,flag=0 ;
    TempSol *tempSol=newTempSol(sol);
    float best= benchmarkSolution(sol,sol->exams);
    float delta,T;
    for(j=0;j<sol->E;j++){
        tempSol->temporarySolution[j]=sol->exams[j];
    }

    startTime=time(NULL);
	T=T0;
	for (i = 0; T > 0.001 && (time(NULL)-startTime)<maxTime;) {
		e1 = random(0, sol->E - 1);
		e2= random(0, sol->E-1);
		if (benchmarkSolution(sol, tempSol->temporarySolution) < best && isFeasible(sol,tempSol->temporarySolution)) {
			for (i = 0; i < sol->E; i++) {
				sol->exams[i] = tempSol->temporarySolution[i];
			}
			best = benchmarkSolution(sol, sol->exams);
		}
		if (flag<turn || isFeasibleSwap(sol, tempSol->temporarySolution, e1, e2)) {
			i++;
			delta = benchmarkSolutionSwap(sol, tempSol->temporarySolution, e1, e2);
			if (delta < 0 || fate(10000 * p(delta, T))) {
				if(!isFeasibleSwap(sol,tempSol->temporarySolution,e1,e2)) flag++;
				swapExam(tempSol->temporarySolution,e1,e2);
				//if(delta>0) T*=alfa;
#ifdef VERBOSE_ANNEALING
				printf("\nConf:%d/%d T:%.4f,%d<->%d(%.3f,%.3f,%.3f)",flag,turn, T, e1, e2, delta,
					   benchmarkSolution(sol, tempSol->temporarySolution), best);
#endif
			}
		}
		if (i % L == 0) {
			T *= alfa;
		}
	}
	extendSol(sol,tempSol);
	findFeasibleSolution(sol,tempSol,maxTime-(time(NULL)-startTime));
	if(arrayEquals(tempSol->temporarySolution,sol->exams,sol->E)) return;
	localSearchTemp(sol,tempSol->temporarySolution,maxTime-(time(NULL)-startTime));
	if (benchmarkSolution(sol, tempSol->temporarySolution) < best && isFeasible(sol,tempSol->temporarySolution)) {
		for (i = 0; i < sol->E; i++) {
			sol->exams[i] = tempSol->temporarySolution[i];
		}
	}
}