//
// Created by munfo on 21/12/2017.
//

#include "greedySlots.h"
#include "utils.h"
#include <malloc.h>
#include <stdio.h>



void greedySlots(dataStructure* sol, int maxTime){
	int* temp=malloc(sizeof(int)*sol->E);
	int* usedFrom=malloc(sizeof(int)*sol->timeSlots);
	int* usedTo=malloc(sizeof(int)*sol->timeSlots);
	for(int i;i<sol->E;i++){
		temp[i]=-1;
	}
	for(int i;i<sol->timeSlots;i++){
		usedFrom[i]=0;
		usedTo[i]=0;
	}
	struct {
		int from;
		int to;
	}best;
	float bestCost=100000000,currentCost;
	int count,from,to;
	for(count=0;count<sol->timeSlots;count++){
		bestCost=100000000;
		for(from=1;from<=sol->timeSlots;from++){
			if(usedFrom[from]==1) continue;
			for(to=1;to<=sol->timeSlots;to++){
				if(usedTo[to]==1) continue;
				swapSlotsGreedy(sol, temp, from, to);
				currentCost=benchmarkSolution(sol,temp);
				if(currentCost<bestCost) {
					bestCost=currentCost;
					best.from = from;
					best.to = to;
				}
				backtrackGreedyslots(sol,temp,to);
			}
		}
		swapSlotsGreedy(sol,temp,best.from,best.to);
		usedFrom[best.from]=1;
		usedTo[best.to]=1;
#ifdef VERBOSE_GREEDY_SLOTS
		printf("\n%d->%d(%.3f,%d)",best.from,best.to,benchmarkSolution(sol,temp),isFeasible(sol,temp));
#endif
	}
	if(benchmarkSolution(sol,temp)<benchmarkSolution(sol,sol->exams) && isFeasible(sol,temp)) copyArray(sol->exams,temp,sol->E);
}

void swapSlotsGreedy(const dataStructure *sol, int *temp, int from, int to) {
	for(int k=0; k < sol->E; k++){
					if(sol->exams[k]==from) temp[k]=to;
				}
}


void backtrackGreedyslots(const dataStructure *sol, int *temp,int to) {
	for(int k=0; k < sol->E; k++){
		if(temp[k]==to) temp[k]=-1;
	}
}