/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
*                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include "ConflictStructure.h"

ConflictStructure* newConflictStructure (int length) {
    int i;
    ConflictStructure *cf = malloc(sizeof(ConflictStructure));

    cf->length = length;
    cf->ConflictArrayLength = 0;
    cf->nAltern = 0;
    cf->numConflictForAlternative = malloc(length * length * sizeof(int));
    cf->ConflictArray = malloc(length * length * sizeof(conflict*));
    cf->alternatives = malloc(length * length * sizeof(int*));
    for (i = 0; i < length * length; ++i) cf->alternatives[i] = malloc(length * sizeof(int));
    for (i = 0; i < length * length; ++i) cf->ConflictArray[i] = malloc(sizeof(conflict));
    for (i = 0; i < length * length; ++i) cf->numConflictForAlternative[i] = 0;
    
    return cf;
}

void freeConflictStructure (ConflictStructure *cf) {
    int i;
    for (i = 0; i < cf->length * cf->length; ++i) free(cf->alternatives[i]);
    for (i = 0; i < cf->length * cf->length; ++i) free(cf->ConflictArray[i]);
    free(cf->alternatives);
    free(cf->ConflictArray);
    free(cf->numConflictForAlternative);
    free(cf);
}

void findConflict (ConflictStructure *cf, dataStructure *sol, TempSol *Tsol) {
    int **adjM = GraphGetAdjMatrix(sol->g), i, j;

    for (i = 0, Tsol->tempSolConflicts = 0; i < GraphGetV(sol->g) - 1; ++i)
        for (j = i + 1; j < GraphGetV(sol->g); ++j) {
            if (adjM[i][j] > 0 && Tsol->temporarySolution[i] == Tsol->temporarySolution[j]) {
                // add conflict
                cf->ConflictArray[cf->ConflictArrayLength]->exam1 = i;
                cf->ConflictArray[cf->ConflictArrayLength]->exam2 = j;
                cf->ConflictArrayLength++;
                Tsol->tempSolConflicts++;
            }
        }
}

// based on the conflicts of the current solution generate alternative solutions
void generateAlternatives (ConflictStructure *cf, TempSol *Tsol) {
    int i, j, nAltern;

    for (i = 0, cf->nAltern = 0, nAltern = 0; i < cf->ConflictArrayLength; ++i) {
        for (j = 0; j < Tsol->currentTimeSlot; ++j) {
            if (j + 1 != Tsol->temporarySolution[cf->ConflictArray[i]->exam1]) {
                copyArray(cf->alternatives[nAltern], Tsol->temporarySolution, cf->length);
                cf->alternatives[nAltern][cf->ConflictArray[i]->exam1] = j + 1;
                nAltern++;
            }
        }
        for (j = 0; j < Tsol->currentTimeSlot; ++j) {
            if (j + 1 != Tsol->temporarySolution[cf->ConflictArray[i]->exam2]) {
                copyArray(cf->alternatives[nAltern], Tsol->temporarySolution, cf->length);
                cf->alternatives[nAltern][cf->ConflictArray[i]->exam2] = j + 1;
                nAltern++;
            }
        }
    }

    cf->nAltern = nAltern;
}

int findBestAlternative (ConflictStructure *cf, TabuList *TL, dataStructure *sol, TempSol *Tsol) {
    int **adjM = GraphGetAdjMatrix(sol->g), i, j, idxWithTabu = 0, idxWOTabu = 0, k;

    for (k = 0; k < cf->nAltern; ++k) {
        cf->numConflictForAlternative[k] = 0;
        for (i = 0; i < GraphGetV(sol->g) - 1; ++i)
            for (j = i + 1; j < GraphGetV(sol->g) - 1; ++j) {
                if (adjM[i][j] > 0 && cf->alternatives[k][i] == cf->alternatives[k][j]) {
                    // add conflict
                    cf->numConflictForAlternative[k]++;
                }
            }
    }

    // find min conflict not considering tabu
    for (k = 0, idxWithTabu = 0; k < cf->nAltern; ++k) {
        if (cf->numConflictForAlternative[k] < cf->numConflictForAlternative[idxWithTabu]) {
            idxWithTabu = k;
        }
    }

    // find min conflict considering tabu moves
    for (k = 0, idxWOTabu = 0; k < cf->nAltern; ++k) {
        if (!isTabu(TL, Tsol, cf->alternatives[k], sol))
            if (cf->numConflictForAlternative[k] < cf->numConflictForAlternative[idxWOTabu]) {
                idxWOTabu = k;
            }
    }

    if (Tsol->numConflictBestSolution > cf->numConflictForAlternative[idxWithTabu])
        return idxWithTabu;
    return idxWOTabu;
}