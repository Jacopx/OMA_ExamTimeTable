/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "ADTgraph.h"
#include "ST.h"
#include "verbose.h"

#ifndef OMA_EXAMTIMETABLE_UTILS_H
#define OMA_EXAMTIMETABLE_UTILS_H




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
float benchmarkSolutionDeltaMove(dataStructure *solution,int * testSol,int exam, int oldSlot,int newSlot);
int isFeasible(dataStructure * ,const int *);
int isFeasibleThis(dataStructure * ,const int *,int exam,int slot);
void findFeasibleSolution (dataStructure *solution,TempSol *Tsol);
void copyArray (int *s1, const int *s2, int l);
void findFeasibleGreedyCi(dataStructure *, TempSol *);
#endif //OMA_EXAMTIMETABLE_UTILS_H
