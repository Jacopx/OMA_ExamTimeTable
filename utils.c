/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#define ITER 195 // 7
#define TABU_LIST_SIZE 200 // 14


typedef struct {        // define the conflict between 2 exams
    int exam1, exam2;
} conflict;

typedef struct {
    int exam; // the exam for which the move is tabu
    int e1, e2; // tabu move is e1 -> e2, ie from timeslot e1 to e2
    int iteration; // how many iteration the move is considered a tabu
} tabuMove;

typedef struct {
    tabuMove **list; // list of tabu moves
    int nMoves; // current number of moves into the list
} TabuList;

typedef struct {
    conflict **ConflictArray;           // array of conflict between 2 exams
    int numConflict;                    // len of ConflictArray
    int nAltern;                        // number of alternatives
    int *numConflictsForAlternatives;   // number of conflict for the corresponding alternative
    int **alternatives;                 // array of alternative solutions (obtained swapping a slot of 1 exams at time)
} ConflictStructure;

int benchmarkSolution (Graph g, int *sol, int S) {
    int i, j, d, penalty, V = GraphGetV(g);
    int **adjM = GraphGetAdjMatrix(g);

    for (i = 0, penalty = 0; i < V - 1; ++i)
        for (j = i + 1; j < V; ++j) {
            //@TODO calculate slot distance (d) between exams based on structure of sol
            penalty += ( pow(2, 5 - d) * adjM[i][j]) / S;
        }

    return penalty;
}

void findConflict (ConflictStructure *cf, int *tempSol, dataStructure *sol) {
    int **adjM = GraphGetAdjMatrix(sol->g), i, j;

    for (i = 0; i < GraphGetV(sol->g) - 1; ++i)
        for (j = i + 1; j < GraphGetV(sol->g); ++j) {
            if (adjM[i][j] > 0 && tempSol[i] == tempSol[j]) {
                // add conflict
                cf->ConflictArray[cf->numConflict]->exam1 = i;
                cf->ConflictArray[cf->numConflict]->exam2 = j;
                cf->numConflict++;
            }
        }
}

void copyArray (int *s1, int *s2, int l) {
    int i;
    for (i = 0; i < l; ++i) s1[i] = s2[i];
}

int isTabu (TabuList *TL, int *tempSol, int *alternative, dataStructure *solution) {
    int i, e1 = -1, e2 = -1, exam = -1;

    for (i = 0; i < solution->E; ++i) {
        if (tempSol[i] != alternative[i]) {
            e1 = tempSol[i];
            e2 = alternative[i];
            exam = i;
        }
    }

    if (e1 != -1 && e2 != -1 && exam != -1)
        for (i = 0; i < TL->nMoves; ++i) {
            if (TL->list[i]->e1 == e1 && TL->list[i]->e2 == e2 && TL->list[i]->iteration > 0 && TL->list[i]->exam == exam) {
                // printf("tabu between: %d, %d for exam %d\n", e1, e2, exam);
                return 1;
            }
        }

    return 0;
}

int calculateConflict (ConflictStructure *cf, dataStructure *sol, int *tempSol, TabuList *TL) {
    int **adjM = GraphGetAdjMatrix(sol->g), i, j, idxWithTabu = 0, idxWOTabu = 0, k;

    for (k = 0; k < cf->nAltern; ++k) {
        cf->numConflictsForAlternatives[k] = 0;
        for (i = 0; i < GraphGetV(sol->g) - 1; ++i)
            for (j = i + 1; j < GraphGetV(sol->g) - 1; ++j) {
                if (adjM[i][j] > 0 && cf->alternatives[k][i] == cf->alternatives[k][j]) {
                    // add conflict
                    cf->numConflictsForAlternatives[k]++;
                }
            }
    }
    /*printf("---tabulist\n");
    for (i = 0; i < TL->nMoves; ++i) {
        printf("exam: %d, e1: %d, e2: %d, iter: %d\n", TL->list[i]->exam, TL->list[i]->e1, TL->list[i]->e2, TL->list[i]->iteration);
    } printf("----\n");*/
    // find min conflict not considering tabu
    for (k = 0, idxWithTabu = 0; k < cf->nAltern; ++k) {
        if (cf->numConflictsForAlternatives[k] < cf->numConflictsForAlternatives[idxWithTabu]) {
            idxWithTabu = k;
        }
    }

    // find min conflict considering tabu moves
    for (k = 0, idxWOTabu = 0; k < cf->nAltern; ++k) {
        if (!isTabu(TL, tempSol, cf->alternatives[k], sol))
            if (cf->numConflictsForAlternatives[k] < cf->numConflictsForAlternatives[idxWOTabu]) {
                idxWOTabu = k;
            }
    }

    if (cf->numConflictsForAlternatives[idxWithTabu] < cf->numConflictsForAlternatives[idxWOTabu]) {
        /*for (i = 0; i < TL->nMoves; ++i) {
            if (TL->list[i]->iteration > 0) TL->list[i]->iteration--;
    }*/
        //return idxWithTabu;
    }
    return idxWOTabu;
}

void generateAlternatives (ConflictStructure *cf, int currentSlots, int *tempSol, int length) {
    int i, j, k;

    for (i = 0, k = 0; i < cf->numConflict; ++i) {
        for (j = 0; j < currentSlots; ++j) {
            if (j + 1 != tempSol[cf->ConflictArray[i]->exam1]) {
                copyArray(cf->alternatives[k], tempSol, length);
                cf->alternatives[k][cf->ConflictArray[i]->exam1] = j + 1;
                k++;
            }
        }
        for (j = 0; j < currentSlots; ++j) {
            if (j + 1 != tempSol[cf->ConflictArray[i]->exam2]) {
                copyArray(cf->alternatives[k], tempSol, length);
                cf->alternatives[k][cf->ConflictArray[i]->exam2] = j + 1;
                k++;
            }
        }
    }

    cf->nAltern = k;
}

void addTabu (TabuList *TL, int *tempSol, int *alternative, int l, int slot) {
    int i, e1 = -1, e2 = -1, exam = -1, found;

    for (i = 0; i < TL->nMoves; ++i) {
        if (TL->list[i]->iteration > 0) TL->list[i]->iteration--;
    }

    for (i = 0; i < l; ++i) {
        if (tempSol[i] != alternative[i]) {
            e1 = tempSol[i];
            e2 = alternative[i];
            exam = i;
        }
    }

    printf("current move: %d, %d for exam %d\n", e1, e2, exam);
    if (e1 != -1 && e2 != -1 && exam != -1)
        for (i = 0, found = 0; i < TL->nMoves && !found; ++i) {
            if (TL->list[i]->iteration == 0) {
                TL->list[i]->e1 = e2;
                TL->list[i]->e2 = e1;
                TL->list[i]->exam = exam;
                TL->list[i]->iteration = ITER;
                found = 1;
            }
        }
    if (slot <= 15) {
        for (i = 0, found = 0; i < TL->nMoves && !found; ++i) {
            if (TL->list[i]->iteration == 0) {
                TL->list[i]->e1 = e1;
                TL->list[i]->e2 = e2;
                TL->list[i]->exam = exam;
                TL->list[i]->iteration = ITER;
                found = 1;
            }
        }
    }
}

void findFeasibleSolution (dataStructure *solution) {
    int *tempSol, i, currentSlot, n, j, c;
    ConflictStructure *cf;
    TabuList *TL;

    // initialization
    currentSlot = solution->E;
    tempSol = malloc(solution->E * sizeof(int));
    for (i = 0; i < solution->E; ++i) tempSol[i] = i + 1;

    TL = malloc(sizeof(TabuList));
    TL->nMoves = TABU_LIST_SIZE;
    TL->list = malloc(TABU_LIST_SIZE * sizeof(tabuMove));
    for (i = 0; i < TABU_LIST_SIZE; ++i) {
        TL->list[i] = malloc(sizeof(tabuMove));
        TL->list[i]->iteration = 0;
    }

    cf = malloc(sizeof(ConflictStructure));
    cf->numConflict = 0;
    cf->nAltern = 0;
    cf->numConflictsForAlternatives = malloc(solution->E * solution->E * sizeof(int));
    cf->ConflictArray = malloc(solution->E * solution->E * sizeof(conflict*));
    cf->alternatives = malloc(solution->E * solution->E * sizeof(int*));
    for (i = 0; i < solution->E * solution->E; ++i) cf->alternatives[i] = malloc(solution->E * sizeof(int));
    for (i = 0; i < solution->E * solution->E; ++i) cf->ConflictArray[i] = malloc(sizeof(conflict));
    for (i = 0; i < solution->E * solution->E; ++i) cf->numConflictsForAlternatives[i] = 0;


    while (currentSlot + 1 > solution->timeSlots) {
        // check feasibility, or find conflicts
        n = 1;
        while (n != 0) {
            cf->numConflict = 0;
            findConflict(cf, tempSol, solution);
            if (cf->numConflict != 0) {
                cf->nAltern = 0;
                generateAlternatives(cf, currentSlot, tempSol, solution->E);
                j = calculateConflict(cf, solution, tempSol, TL); // j = index of best alternative
                n = cf->numConflictsForAlternatives[j];
                //save this move as tabu
                addTabu(TL, tempSol, cf->alternatives[j], solution->E, currentSlot);

                copyArray(tempSol, cf->alternatives[j], solution->E);
            } else n = 0;
            printf("slot: %d, conflicts: %d\n", currentSlot, n);
            for (i = 0; i < TL->nMoves; ++i) {
                // printf("e1: %d, e2: %d, iter: %d\n", TL->list[i]->e1, TL->list[i]->e2, TL->list[i]->iteration);
            }
        }

        if (currentSlot != solution->timeSlots) {
            c = tempSol[solution->E - 1]; // timeSlot of last exam
            if (c != currentSlot) {
                for (i = 0; i < solution->E; ++i) {
                    if (tempSol[i] == c) {
                        tempSol[i] = currentSlot;
                    } else if (tempSol[i] == currentSlot) {
                        tempSol[i] = c;
                    }
                }
            }

            for (i = 0; i < solution->E; ++i) {
                if (tempSol[i] == currentSlot)
                    tempSol[i]--;
            }
        }
        currentSlot--;
    }

    copyArray(solution->exams, tempSol, solution->E);

    // free everything
    free(tempSol);
    for (i = 0; i < solution->E * solution->E; ++i) free(cf->alternatives[i]);
    for (i = 0; i < solution->E * solution->E; ++i) free(cf->ConflictArray[i]);
    for (i = 0; i < 2 * ITER; ++i) free(TL->list[i]);
    free(TL);
    free(cf->alternatives);
    free(cf->ConflictArray);
    free(cf->numConflictsForAlternatives);
    free(cf);
}