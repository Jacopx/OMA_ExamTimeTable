/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <math.h>
#include "utils.h"
#include "ADTgraph.h"

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