/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "ADTgraph.h"

typedef struct {        // define the conflict between 2 exams
    int exam1, exam2;
} conflict;

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

int calculateConflict (ConflictStructure *cf, dataStructure *sol) {
    int **adjM = GraphGetAdjMatrix(sol->g), i, j = 0, k;

    for (k = 0; k < cf->nAltern; ++k)
        for (i = 0; i < GraphGetV(sol->g) - 1; ++i)
            for (j = i + 1; j < GraphGetV(sol->g) - 1; ++j) {
                if (adjM[i][j] > 0 && cf->alternatives[k][i] == cf->alternatives[k][j]) {
                    // add conflict
                    cf->numConflictsForAlternatives[k]++;
                }
            }
    // find min
    for (k = 0, j = 0; k < cf->nAltern; ++k) {
        if (cf->numConflictsForAlternatives[k] < cf->numConflictsForAlternatives[j]) {
            j = k;
        }
    }
    return j;
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

void findFeasibleSolution (dataStructure *solution) {
    int *tempSol, i, currentSlot, n, j;
    ConflictStructure *cf;

    // initialization
    currentSlot = solution->E;
    tempSol = malloc(solution->E * sizeof(int));
    for (i = 0; i < solution->E; ++i) tempSol[i] = i + 1;

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
        // @TODO add tabu list check

        n = 1;
        while (n != 0) {
            cf->numConflict = 0;
            findConflict(cf, tempSol, solution);
            if (cf->numConflict != 0) {
                cf->nAltern = 0;
                generateAlternatives(cf, currentSlot, tempSol, solution->E);
                j = calculateConflict(cf, solution); // j = index of best alternative
                n = cf->numConflictsForAlternatives[j];
                copyArray(tempSol, cf->alternatives[j], solution->E);
            } else n = 0;
            printf("slot: %d, conflicts: %d\n", currentSlot, n);
        }

        if (currentSlot != solution->timeSlots)
            for (i = 0; i < solution->E; ++i) {
                if (tempSol[i] == currentSlot)
                    tempSol[i]--;
            }
        currentSlot--;
    }

    copyArray(solution->exams, tempSol, solution->E);

    // free everything
    free(tempSol);
    for (i = 0; i < solution->E * solution->E; ++i) free(cf->alternatives[i]);
    for (i = 0; i < solution->E * solution->E; ++i) free(cf->ConflictArray[i]);
    free(cf->alternatives);
    free(cf->ConflictArray);
    free(cf->numConflictsForAlternatives);
    free(cf);
}