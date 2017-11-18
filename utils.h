/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "ADTgraph.h"
#include "ST.h"

#ifndef OMA_EXAMTIMETABLE_UTILS_H
#define OMA_EXAMTIMETABLE_UTILS_H

typedef  struct {
    int *exams;
    int E, timeSlots;
    Graph g;
    st tab;
} dataStructure;

int benchmarkSolution (Graph g, int *sol, int S);
void findFeasibleSolution (dataStructure *solution);

#endif //OMA_EXAMTIMETABLE_UTILS_H
