//
// Created by munfo on 21/12/2017.
//

#include "greedySlots.h"
#include "utils.h"
#include <malloc.h>
#include <stdio.h>
#include "rng.h"
#include "local.h"



void greedySlots(dataStructure* sol, int maxTime){ //todo This one is broken
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
	int startTime=time(NULL);
	for(count=0;count<sol->timeSlots;count++){
		bestCost=100000000;
		for(from=1;from<=sol->timeSlots;from++){
			if(usedFrom[from]==1) continue;
			for(to=1;to<=sol->timeSlots;to++){
				if(time(NULL)-startTime>=maxTime) return;
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
		printf("\n%d->%d(%.3f)",best.from,best.to,benchmarkSolution(sol,temp));
#endif
	}

	if(benchmarkSolution(sol,temp)<benchmarkSolution(sol,sol->exams) && isFeasible(sol,temp)) copyArray(sol->exams,temp,sol->E);
}

void greedySlotsOrdered(dataStructure *sol, int maxTime){
	int* temp=malloc(sizeof(int)*sol->E);
	int* usedFrom=malloc(sizeof(int)*sol->timeSlots);
	int* usedTo=malloc(sizeof(int)*sol->timeSlots);
	int* order=malloc(sizeof(int)*sol->timeSlots);
	for(int i=0;i<sol->E;i++){
		temp[i]=-1;
	}
	for(int i=0;i<sol->timeSlots;i++){
		usedFrom[i]=0;
		usedTo[i]=0;
		order[i]=i+1;
	}
	//shuffle
/*	for(int cicle=0;cicle<sol->timeSlots*10;cicle++){
		int a=random(0,sol->timeSlots-1);
		int b=random(0,sol->timeSlots-1);
		int t=order[a];
		order[a]=order[b];
		order[b]=t;
	}*/
	int startTime=time(NULL);
	orderSlots(sol,order);
	struct {
		int from;
		int to;
	}best;
	float bestCost=100000000,currentCost;
	int count,from,to,k;
	k=0;
	for(best.from=order[0];k<sol->timeSlots;best.from=order[++k]){
		bestCost=100000000;
		for(to=1;to<=sol->timeSlots;to++){
			if(time(NULL)-startTime>=maxTime) return;
			if(usedTo[to]==1) continue;
			swapSlotsGreedy(sol, temp, best.from, to);
			currentCost=benchmarkSolution(sol,temp);
			if(currentCost<bestCost) {
				bestCost=currentCost;
				best.to = to;
			}
			backtrackGreedyslots(sol,temp,to);
		}
		swapSlotsGreedy(sol,temp,best.from,best.to);
		usedFrom[best.from]=1;
		usedTo[best.to]=1;
#ifdef VERBOSE_GREEDY_SLOTS
		printf("\n%d->%d(%.3f)",best.from,best.to,benchmarkSolution(sol,temp));
#endif
	}
	if(benchmarkSolution(sol,temp)<benchmarkSolution(sol,sol->exams) && isFeasible(sol,temp)) copyArray(sol->exams,temp,sol->E);
}
void greedySlotsShuffle(dataStructure *sol, int maxTime){
	int* temp=malloc(sizeof(int)*sol->E);
	int* usedFrom=malloc(sizeof(int)*sol->timeSlots);
	int* usedTo=malloc(sizeof(int)*sol->timeSlots);
	int* order=malloc(sizeof(int)*sol->timeSlots);
	for(int i=0;i<sol->E;i++){
		temp[i]=-1;
	}
	for(int i=0;i<sol->timeSlots;i++){
		usedFrom[i]=0;
		usedTo[i]=0;
		order[i]=i+1;
	}
	//shuffle
	for(int cicle=0;cicle<sol->timeSlots*10;cicle++){
		int a=random(0,sol->timeSlots-1);
		int b=random(0,sol->timeSlots-1);
		int t=order[a];
		order[a]=order[b];
		order[b]=t;
	}
	int startTime=time(NULL);
	struct {
		int from;
		int to;
	}best;
	float bestCost=100000000,currentCost;
	int count,from,to,k;
	k=0;
	for(best.from=order[0];k<sol->timeSlots;best.from=order[++k]){
		bestCost=100000000;
		for(to=1;to<=sol->timeSlots;to++){
			if(time(NULL)-startTime>=maxTime) return;
			if(usedTo[to-1]==1) continue;
			swapSlotsGreedy(sol, temp, best.from, to);
			currentCost=benchmarkSolution(sol,temp);
			if(currentCost<bestCost) {
				bestCost=currentCost;
				best.to = to;
			}
			backtrackGreedyslots(sol,temp,to);
		}
		swapSlotsGreedy(sol,temp,best.from,best.to);
		usedFrom[best.from-1]=1;
		usedTo[best.to-1]=1;
#ifdef VERBOSE_GREEDY_SLOTS
		printf("\n%d->%d(%.3f)",best.from,best.to,benchmarkSolution(sol,temp));
#endif
	}
	localSearchTemp(sol,temp,maxTime-(time(NULL)-startTime));
	if(benchmarkSolution(sol,temp)<benchmarkSolution(sol,sol->exams) && isFeasible(sol,temp)) copyArray(sol->exams,temp,sol->E);
}
void swapSlotsGreedy(const dataStructure *sol, int *temp, int from, int to) {
	for(int k=0; k < sol->E; k++){
					if(sol->exams[k]==from) temp[k]=to;
				}
}
void swapSlotsGreedyOnTemp(const dataStructure *sol, int *temp, int from, int to) {
	for(int k=0; k < sol->E; k++){
					if(temp[k]==from) temp[k]=to;
				}
}


void backtrackGreedyslots(const dataStructure *sol, int *temp,int to) {
	for(int k=0; k < sol->E; k++){
		if(temp[k]==to) temp[k]=-1;
	}
}

void orderSlots(dataStructure *sol,int *order){
	for(int i=0;i<sol->timeSlots;i++){
		order[i]=i+1;
	}
	int* temp=malloc(sizeof(int)*sol->E);
	for(int i=0;i<sol->E;i++){
		temp[i]=-1;
	}
	float a,b;
	copyArray(temp,sol->exams,sol->E);
	for(int i=0;i<sol->timeSlots;i++){
		for(int j=0;j<sol->timeSlots;j++){
			swapSlotsGreedyOnTemp(sol,temp,j,-1);
			a=benchmarkSolution(sol,temp);
			copyArray(temp,sol->exams,sol->E);
			swapSlotsGreedyOnTemp(sol,temp,j+1,-1);
			b=benchmarkSolution(sol,temp);
			copyArray(temp,sol->exams,sol->E);
			;
			if(b<a){
				int t=order[j];
				order[j]=order[j+1];
				order[j+1]=t;
			}
		}
	}
	free(temp);
}