/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "TabuList.h"
#include "ConflictStructure.h"
#include "rng.h"
#define MAX_ITERATION 200


float benchmarkSolution (dataStructure * solution) {
    int i, j, V = GraphGetV(solution->g), d;
    int **adjM = GraphGetAdjMatrix(solution->g);
    float penalty;

    for (i = 0, penalty = 0; i < V - 1; ++i)
        for (j = i + 1; j < V; ++j) {
            if(solution->exams[i]==-1 || solution->exams[j]==-1) continue; //for partial solution
            d = abs(solution->exams[i] - solution->exams[j]);
            if (d <= 5 && adjM[i][j] != 0)
                penalty += (float)pow(2, 5 - d) * (float)adjM[i][j];
        }

    return penalty / solution->S;
}

int isFisible(dataStructure * solution){
    int i, j, V = GraphGetV(solution->g);
    int **adjM = GraphGetAdjMatrix(solution->g);

    for (i = 0; i < V - 1; ++i) {
        for (j = i + 1; j < V; ++j) {
            if (solution->exams[i] == -1 || solution->exams[j] == -1) continue; //for partial solution
            if(solution->exams[i]==solution->exams[j] && adjM[i][j]>0) return 0;
        }
    }
return 1;
}

void copyArray (int *s1, const int *s2, int l) {
    int i;
    for (i = 0; i < l; ++i) s1[i] = s2[i];
}

TempSol* newTempSol (int nExams) {
    int i;
    TempSol *ts = malloc(sizeof(TempSol));
    ts->length = nExams;
    ts->currentTimeSlot = nExams;
    ts->tempSolConflicts = nExams;
    ts->numConflictBestSolution = nExams; // i.e. number of conflict of current best solution
    ts->temporarySolution = malloc(nExams * sizeof(int));
    for (i = 0; i < nExams; ++i) ts->temporarySolution[i] = i + 1;
    return ts;
}

void freeTempSol (TempSol *ts) {
    free(ts->temporarySolution);
    free(ts);
}

void findFeasibleSolution (dataStructure *solution) {
    int i, c = 1, bestAlternativeIndex, iteration, isLooping, **backupSolution, diff = solution->E, var;
    TempSol *Tsol;
    TabuList *TL;
    ConflictStructure *cf;

    backupSolution = malloc(solution->E * sizeof(int*));
    for (i = 0; i < solution->E; ++i) backupSolution[i] = malloc(solution->E * sizeof(int));

    // initialization
    Tsol = newTempSol(solution->E);
    TL = newTabuList();
    cf = newConflictStructure(solution->E);

    while (Tsol->currentTimeSlot + 1 > solution->timeSlots) {
        // check feasibility, or find conflicts
        Tsol->tempSolConflicts = 10000;
        Tsol->numConflictBestSolution = 10000;

        iteration = 0;
        isLooping = 0;
        while (Tsol->numConflictBestSolution != 0) {
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
                printf("nooo\n");
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

    // save founded solution
    copyArray(solution->exams, Tsol->temporarySolution, solution->E);

    for (i = 0; i < solution->E; ++i) free(backupSolution[i]);
    free(backupSolution);

    // free everything
    freeTempSol(Tsol);
    freeTabuList(TL);
    freeConflictStructure(cf);
}

void findFeasibleGreedyCi(dataStructure* solution){
    int i,j,more;
    struct {
        int exam;
        int slot;
    }best;

    for(i=0;i<solution->E;i++){
        solution->exams[i]=-1;
    }

    more=0;

    for(i=0;i<solution->E;i++) {
        best.exam = -1;
        best.slot = 10000000;
        for (j = 1; j <= solution->timeSlots+more; j++) {
           solution->exams[i] = j;
           if(isFisible(solution) && j < best.slot) {
               best.exam = i;
               best.slot = j;
           }
           solution->exams[i] = -1;
           if(best.slot==0) break;
        }
        if(best.exam==-1){
            more++;
            i--;
        }
        solution->exams[best.exam]=best.slot;
#ifdef VERBOSE_GREEDY_CI
        printf("\n%d->%d(more:%d) %d/%d",best.exam,best.slot,more,i+1,solution->E);
#endif
    }
}