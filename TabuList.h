/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
*                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef OMA_EXAMTIMETABLE_TABULIST_H
#define OMA_EXAMTIMETABLE_TABULIST_H

#include "utils.h"

typedef struct {
    int exam; // the exam for which the move is tabu
    int e1, e2; // tabu move is e1 <-> e2
    int iteration; // how many iteration the move is considered a tabu
} tabuMove;

typedef struct {
    tabuMove **list; // list of tabu moves
    int listLength; // current number of moves into the list
    int iteration;
} TabuList;

TabuList* newTabuList ();
void freeTabuList (TabuList *tl);
void addTabu (TabuList *TL, TempSol *Tsol, const int *alternative);
int isTabu (TabuList *TL, TempSol *Tsol, const int *alternative, dataStructure *solution);

#endif //OMA_EXAMTIMETABLE_TABULIST_H
