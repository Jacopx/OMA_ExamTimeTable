//
// Created by munfo on 10/12/2017.
//

#include "annealing.h"
#include "rng.h"
#include "local.h"
#include <stdio.h>

#define p(delta,T) (exp(-(delta)/(T)))


const float alfa = 0.99;
const float T0 = 100;
const int L=100;

const int maxTurn=100;

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
				if(delta>0) T*=alfa;
				printf("\nConf:%d/%d T:%.4f,%d->%d(%.3f,%.3f,%.3f)",flag,turn, T, e, s, delta,
				       benchmarkSolution(sol, tempSol->temporarySolution), best);
			}
		}
		if (i % L == 0) {
			T *= alfa;
		}
	}
	findFeasibleSolution(sol,tempSol,maxTime-(time(NULL)-startTime));
	if (benchmarkSolution(sol, tempSol->temporarySolution) < best && isFeasible(sol,tempSol->temporarySolution)) {
		for (i = 0; i < sol->E; i++) {
			sol->exams[i] = tempSol->temporarySolution[i];
		}
	}
}