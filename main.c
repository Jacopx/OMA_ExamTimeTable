/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include "fileManager.h"
#include <time.h>
#include "annealing.h"
#include "local.h"
#include "utils.h"

int main(int argc, char **argv) {
    dataStructure *solution = malloc(sizeof(dataStructure));
    TempSol *temp=NULL;
    static time_t t1, t2;


    if (argc == 4) solution->timeLimit = atoi(argv[argc - 1]);
    else solution->timeLimit = 0;

    t1 = time(0);

    read_Slo(argv[1], solution);
    read_Exm(argv[1], solution);
    read_Stu(argv[1], solution);

    sortBasedOnEdges(solution);
    read_Stu(argv[1], solution);

    temp=newTempSol(solution);
    findFeasibleGreedyCi(solution,temp);
    findFeasibleSolution(solution,temp);
    print_Sol(argv[1], solution);

    simulateAnnealingSearch(solution,200);
    localSearch(solution,1000000);
    print_Sol(argv[1],solution);

    freeTempSol(temp);
    STfree(solution->tab);
    GraphFree(solution->g);
    free(solution->exams);
    free(solution);

    t2 = time(0);
    printf("Finished in %d seconds\n", (int) (t2-t1));
    return 0;
}
