/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
*                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



#include <stdio.h>
#include <stdlib.h>
#include "TabuList.h"

#define ITER 7
#define TABU_LIST_SIZE 1000

TabuList* newTabuList () {
    int i;
    TabuList *tl = malloc(sizeof(TabuList));
    tl->listLength = TABU_LIST_SIZE;
    tl->list = malloc(TABU_LIST_SIZE * sizeof(tabuMove));
    tl->iteration = ITER;

    for (i = 0; i < TABU_LIST_SIZE; ++i) {
        tl->list[i] = malloc(sizeof(tabuMove));
        tl->list[i]->iteration = 0;
    }

    return tl;
}

void freeTabuList (TabuList *tl) {
    int i;
    for (i = 0; i < 2 * ITER; ++i) free(tl->list[i]);
    free(tl);
}

void addTabu (TabuList *TL, TempSol *Tsol, const int *alternative) {
    int i, e1 = -1, e2 = -1, exam = -1, found;

    for (i = 0; i < TL->listLength; ++i) {
        if (TL->list[i]->iteration > 0) TL->list[i]->iteration--;
    }

    for (i = 0; i < Tsol->length; ++i) {
        if (Tsol->temporarySolution[i] != alternative[i]) {
            e1 = Tsol->temporarySolution[i];
            e2 = alternative[i];
            exam = i;
        }
    }
#ifdef VERBOSE_TABU_LIST
    printf("current move: exam %d: %d->%d\n", exam, e1, e2);
#endif
    for (i = 0, found = 0; i < TL->listLength && !found; ++i) {
        if (TL->list[i]->iteration == 0) {
            TL->list[i]->e1 = e2;
            TL->list[i]->e2 = e1;
            TL->list[i]->exam = exam;
            TL->list[i]->iteration = TL->iteration;
            found = 1;
        }
    }


}

int isTabu (TabuList *TL, TempSol *Tsol, const int *alternative, dataStructure *solution) {
    int i, e1 = -1, e2 = -1, exam = -1;
    // @TODO pass directly the exams that we want to swap

    for (i = 0; i < solution->E; ++i) {
        if (Tsol->temporarySolution[i] != alternative[i]) {
            e1 = Tsol->temporarySolution[i];
            e2 = alternative[i];
            exam = i;
        }
    }

    for (i = 0; i < TL->listLength; ++i) {
        if (TL->list[i]->iteration > 0 && TL->list[i]->exam == exam && ((TL->list[i]->e1 == e1 && TL->list[i]->e2 == e2) || (TL->list[i]->e1 == e2 && TL->list[i]->e2 == e1)))
            return 1;
    }
    return 0;
}