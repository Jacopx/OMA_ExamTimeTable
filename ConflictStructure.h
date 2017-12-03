/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
*                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef OMA_EXAMTIMETABLE_CONFLICTSTRUCTURE_H
#define OMA_EXAMTIMETABLE_CONFLICTSTRUCTURE_H

#include "utils.h"
#include "TabuList.h"

typedef struct {    // define the conflict between 2 exams
    int exam1, exam2;
} conflict;

typedef struct {
    int length;
    conflict **ConflictArray;       // array of conflict between 2 exams
    int ConflictArrayLength;        // len of ConflictArray
    int nAltern;                    // number of alternatives
    int **alternatives;             // array of alternative solutions (obtained swapping a slot of 1 exams at time)
    int *numConflictForAlternative; // number of conflict for the corresponding alternative
} ConflictStructure;

ConflictStructure* newConflictStructure (int length);
void freeConflictStructure (ConflictStructure *cf);
void findConflict (ConflictStructure *cf, dataStructure *sol, TempSol *Tsol);
void generateAlternatives (ConflictStructure *cf, TempSol *Tsol);
int findBestAlternative (ConflictStructure *cf, TabuList *TL, dataStructure *sol, TempSol *Tsol);

#endif //OMA_EXAMTIMETABLE_CONFLICTSTRUCTURE_H
