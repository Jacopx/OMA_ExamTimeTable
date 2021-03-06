/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "TabuList.h"
#include "ConflictStructure.h"
#include <time.h>

#define MAX_ITERATION 200

float benchmarkSolution (dataStructure * solution,int* testSol) {
    int i, j, V = GraphGetV(solution->g), d;
    int **adjM = GraphGetAdjMatrix(solution->g);
    int penalty;
    const int fastPow[6]={32,16,8,4,2,1};

    for (i = 0, penalty = 0; i < V - 1; ++i)
        for (j = i + 1; j < V; ++j) {
            if(testSol[i]==-1 || testSol[j]==-1) continue; //for partial solution
            d = abs(testSol[i] - testSol[j]);
            if (d <= 5 && adjM[i][j] != 0)
                penalty += fastPow[d] * adjM[i][j];
        }

    return (float)penalty / solution->S;
}

float benchmarkSolutionDeltaMove(dataStructure *solution,int * testSol,int exam, int oldSlot,int newSlot){
    int i, d;
    int **adjM = GraphGetAdjMatrix(solution->g);
    int penalty=0;
    const int fastPow[6]={32,16,8,4,2,1};
    for(i=0;i<solution->E;i++){
        if(i==exam) continue;
        d = abs(testSol[i] - oldSlot);
        if (d<=5 && adjM[i][exam] != 0)
            penalty -= fastPow[d] * adjM[i][exam];
    }
    for(i=0;i<solution->E;i++){
        if(i==exam) continue;
        d = abs(testSol[i] - newSlot);
        if (d<=5 && adjM[i][exam] != 0)
            penalty += fastPow[d] * adjM[i][exam];
    }
    return (float)penalty / solution->S;
}

float benchmarkSolutionSwap(dataStructure* solution,int *testSol,int e1, int e2){
	int oldSlot,newSlot;
	int i, d, temp;
	int **adjM = GraphGetAdjMatrix(solution->g);
	int penalty=0;
	const int fastPow[6]={32,16,8,4,2,1};
	oldSlot=testSol[e1];
	newSlot=testSol[e2];
	for(i=0;i<solution->E;i++){
		if(i==e1) continue;
		d = abs(testSol[i] - oldSlot);
		if (d<=5 && adjM[i][e1] != 0)
			penalty -= fastPow[d] * adjM[i][e1];
	}
	temp=testSol[e2];
	testSol[e2]=testSol[e1];
	for(i=0;i<solution->E;i++){
		if(i==e1) continue;
		d = abs(testSol[i] - newSlot);
		if (d<=5 && adjM[i][e1] != 0)
			penalty += fastPow[d] * adjM[i][e1];
	}
	testSol[e2]=temp;
	oldSlot=testSol[e2];
	newSlot=testSol[e1];
	for(i=0;i<solution->E;i++){
		if(i==e2) continue;
		d = abs(testSol[i] - oldSlot);
		if (d<=5 && adjM[i][e2] != 0)
			penalty -= fastPow[d] * adjM[i][e2];
	}
	temp=testSol[e1];
	testSol[e1]=testSol[e2];
	for(i=0;i<solution->E;i++){
		if(i==e2) continue;
		d = abs(testSol[i] - newSlot);
		if (d<=5 && adjM[i][e2] != 0)
			penalty += fastPow[d] * adjM[i][e2];
	}
	testSol[e1]=temp;
	return (float)penalty / solution->S;
}

int isFeasible(dataStructure * solution,const int *testSol){
    int i, j, V = GraphGetV(solution->g);
    int **adjM = GraphGetAdjMatrix(solution->g);

    for (i = 0; i < V - 1; ++i) {
        for (j = i + 1; j < V; ++j) {
            if(testSol[i] == -1 || testSol[j] == -1) continue; //for partial solution
            if(testSol[i]==testSol[j] && adjM[i][j]>0) return 0;
            if(testSol[i]>solution->timeSlots) return 0;
        }
    }
return 1;
}

int isFeasibleThis(dataStructure * solution,const int *testSol,int exam,int slot){
    int i;
    int **adjM = GraphGetAdjMatrix(solution->g);

for(i=0;i<solution->E;i++){
    if(testSol[i]==slot && i!=exam){
        if(adjM[i][exam]>0) return 0;
    }
}
return 1;
}

int isFeasibleSwap(dataStructure * solution,const int * temp,int e1,int e2){
    int **adjM = GraphGetAdjMatrix(solution->g);
    for(int i=0;i<solution->E;i++){
        if(i!=e2 && temp[e2]==temp[i] && adjM[e1][i]!=0) return 0;
        if(i!=e1 && temp[e1]==temp[i] && adjM[e2][i]!=0) return 0;
    }
    return 1;
}

void extendSol(dataStructure* sol,TempSol* temp){
	int i, j, V = GraphGetV(sol->g);
	int **adjM = GraphGetAdjMatrix(sol->g);
	int count=1;
	for (i = 0; i < V - 1; ++i) {
		for (j = i + 1; j < V; ++j) {
			if(temp->temporarySolution[i]==temp->temporarySolution[j] && adjM[i][j]>0){
				temp->temporarySolution[i]=sol->timeSlots+count++;
			}
		}
	}
	temp->currentTimeSlot=sol->timeSlots+count;
	temp->numConflictBestSolution=sol->E;
}

void copyArray (int *s1, const int *s2, int l) {
    int i;
    for (i = 0; i < l; ++i) s1[i] = s2[i];
}

int arrayEquals (const int *s1, const int *s2,int l) {
    for (int i = 0; i < l; ++i) if(s1[i] != s2[i]) return 0;
	return 1;
}

TempSol* newTempSol (dataStructure* sol) {
    int i;
    TempSol *ts = malloc(sizeof(TempSol));
    ts->length = sol->E;
    ts->currentTimeSlot = sol->E;
    ts->tempSolConflicts =sol->E;
    ts->numConflictBestSolution = sol->E; // i.e. number of conflict of current best solution
    ts->temporarySolution = malloc(sol->E * sizeof(int));
    for (i = 0; i < sol->E; ++i) ts->temporarySolution[i] = -1;
    return ts;
}

void freeTempSol (TempSol *ts) {
    free(ts->temporarySolution);
    free(ts);
}

void findFeasibleSolution (dataStructure *solution,TempSol *Tsol,int maxTime) {
    int i, c = 1, bestAlternativeIndex, iteration, isLooping, **backupSolution, diff = solution->E, var;
    TabuList *TL;
    ConflictStructure *cf;
    int startTime=time(NULL);
    backupSolution = malloc(solution->E * sizeof(int*));
    for (i = 0; i < solution->E; ++i) backupSolution[i] = malloc(solution->E * sizeof(int));

    // initialization
    TL = newTabuList();
    cf = newConflictStructure(solution->E);

    while (Tsol->currentTimeSlot + 1 > solution->timeSlots) {
        // check feasibility, or find conflicts
        Tsol->tempSolConflicts = 10000;
        Tsol->numConflictBestSolution = 10000;
        iteration = 0;
        isLooping = 0;
        while (Tsol->numConflictBestSolution != 0) {
	        if(time(NULL)-startTime>=maxTime) return;
            iteration++;
            cf->ConflictArrayLength = 0;
            findConflict(cf, solution, Tsol);
            if (Tsol->tempSolConflicts != 0) {
                generateAlternatives(cf, Tsol);
                bestAlternativeIndex = findBestAlternative(cf, TL, solution, Tsol);
                Tsol->tempSolConflicts = cf->numConflictForAlternative[bestAlternativeIndex];
                //save this move as tabu
                addTabu(TL, Tsol, cf->alternatives[bestAlternativeIndex]);
                copyArray(Tsol->temporarySolution, cf->alternatives[bestAlternativeIndex], solution->E);
            }
            if (Tsol->tempSolConflicts < Tsol->numConflictBestSolution) Tsol->numConflictBestSolution = Tsol->tempSolConflicts;
#ifdef VERBOSE_TABU
            printf("slot: %d, best sol conflicts: %d, current sol conflicts: %d\n", Tsol->currentTimeSlot, Tsol->numConflictBestSolution, Tsol->tempSolConflicts);
#endif
            if (iteration >= 60) {
                TL->iteration += 5;
            }

            if (iteration >= MAX_ITERATION) {
                diff = Tsol->currentTimeSlot;
#ifdef VERBOSE_TABU
               printf("entered into a loop :(\n");
               printf("current time slot: %d\n", Tsol->currentTimeSlot);
#endif
                Tsol->currentTimeSlot++;
                Tsol->numConflictBestSolution = 0;
                copyArray(Tsol->temporarySolution, backupSolution[Tsol->currentTimeSlot - 1], Tsol->length);
#ifdef VERBOSE_TABU
                printf("backtrack to %d\n", Tsol->currentTimeSlot);
                printf("now current sol is: ");
                for (i = 0; i < Tsol->length; ++i) printf("%d ", Tsol->temporarySolution[i]);
                printf("\n");
#endif
                isLooping = 1;
                c++;
#ifdef VERBOSE_TABU
                 printf("trying with slot: %d\n", Tsol->currentTimeSlot - c);
#endif
                // exit(1);
            }
        }

        if (Tsol->currentTimeSlot == diff) {
            // end loop
            c = 1;
        }

        if (!isLooping) copyArray(backupSolution[Tsol->currentTimeSlot - 1], Tsol->temporarySolution, Tsol->length);

        // founded a feasible solution with the current number of timeslot, so decrement timeslot
        // @TODO define this function in a smarter way
        if (Tsol->currentTimeSlot != solution->timeSlots) {
            var = Tsol->currentTimeSlot - c;
            if (var <= 0) {
#ifdef VERBOSE_TABU
                printf("Feasible solution not found!\n");
#endif
                return;
            }
            if (c != Tsol->currentTimeSlot) {
                for (i = 0; i < solution->E; ++i) {
                    if (Tsol->temporarySolution[i] == var) {
                        Tsol->temporarySolution[i] = Tsol->currentTimeSlot;
                    } else if (Tsol->temporarySolution[i] == Tsol->currentTimeSlot) {
                        Tsol->temporarySolution[i] = var;
                    }
                }
            }

            for (i = 0; i < solution->E; ++i) {
                if (Tsol->temporarySolution[i] == Tsol->currentTimeSlot)
                    Tsol->temporarySolution[i]--;
            }
        }
        Tsol->currentTimeSlot--;

        TL->iteration = 7;
    }


    for (i = 0; i < solution->E; ++i) free(backupSolution[i]);
    free(backupSolution);

    // free everything
    freeTabuList(TL);
    freeConflictStructure(cf);
}

void findFeasibleGreedyCi(dataStructure* solution,TempSol *sol){
    int i,j,more;
    struct {
        int exam;
        int slot;
        float val;
    }best;
    more=0;
    for(i=0;i<solution->E;i++) {
        best.exam = -1;
        best.slot = 10000000;
        for (j = 1; j <= solution->timeSlots+more; j++) {
           sol->temporarySolution[i] = j;
           if(isFeasibleThis(solution,sol->temporarySolution,i,j) && j < best.slot) {
               best.exam = i;
               best.slot = j;
           }
            sol->temporarySolution[i] = -1;
           if(best.slot==0) break;
        }
        if(best.exam==-1){
            more++;
            i--;
        }
        sol->temporarySolution[best.exam]=best.slot;
#ifdef VERBOSE_GREEDY_CI
        printf("\n%d->%d(more:%d) %d/%d",best.exam,best.slot,more,i+1,solution->E)  ;
#endif
    }
    sol->currentTimeSlot=solution->timeSlots+more;
}