/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileManager.h"
#include "ADTgraph.h"
#include "ST.h"
#include "utils.h"

int main(int argc, char **argv) {
    dataStructure *solution = malloc(sizeof(dataStructure));

    solution->timeLimit = atoi(argv[argc - 1]);

    read_Slo(argv[1], solution);
    read_Exm(argv[1], solution);
    read_Stu(argv[1], solution);

    sortBasedOnEdges(solution);
    read_Stu(argv[1], solution);

    findFeasibleSolution(solution);
    print_Sol(argv[1], solution);

    STfree(solution->tab);
    GraphFree(solution->g);
    free(solution->exams);
    free(solution);
    return 0;
}