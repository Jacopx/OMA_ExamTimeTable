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

int main() {
    dataStructure *solution = malloc(sizeof(dataStructure));

    read_Slo(fopen("instance01.slo", "r"), &solution->timeSlots);
    read_Exm("instance01.exm", solution);
    read_Stu(file_Open("instance01.stu", "r"), solution);

    findFeasibleSolution(solution);
    print_Sol("instance01.sol", solution);

    STfree(solution->tab);
    GraphFree(solution->g);
    free(solution->exams);
    free(solution);
    return 0;
}