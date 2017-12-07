/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "ADTgraph.h"
#include "ST.h"

#ifndef OMA_EXAMTIMETABLE_UTILS_H
#define OMA_EXAMTIMETABLE_UTILS_H

#define VERBOSE_TABU //there is another one in TabuList
#define VERBOSE_GREEDY_CI

typedef struct {
    int *exams;
    int E, timeSlots;
    Graph g;
    st tab;
    int timeLimit;
    int S;
} dataStructure;

typedef struct {
    int *temporarySolution;
    int currentTimeSlot;
    int tempSolConflicts;
    int numConflictBestSolution;
    int length;
} TempSol;

TempSol* newTempSol (dataStructure* sol);
void freeTempSol (TempSol *ts);
float benchmarkSolution (dataStructure *solution,int * testSol);
int isFeasible(dataStructure * ,const int *);
void findFeasibleSolution (dataStructure *solution,TempSol *Tsol);
void copyArray (int *s1, const int *s2, int l);
void findFeasibleGreedyCi(dataStructure *, TempSol *);
#endif //OMA_EXAMTIMETABLE_UTILS_H
