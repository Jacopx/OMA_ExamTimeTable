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
#include "rng.h"
#include "utils.h"



int main(int argc, char **argv) {
    dataStructure *solution;
    TempSol *temp = NULL;
    static time_t t1, t2;
    int i;
    randomize();


    solution=malloc(sizeof(dataStructure));
    if (argc == 4) solution->timeLimit = atoi(argv[argc - 1]);
    else solution->timeLimit = 0;

    t1 = time(0);

    read_Slo(argv[1], solution);
    read_Exm(argv[1], solution);
    read_Stu(argv[1], solution);

    sortBasedOnEdges(solution);
    read_Stu(argv[1], solution);

    temp = newTempSol(solution);
    findFeasibleGreedyCi(solution, temp);
    findFeasibleSolution(solution, temp,solution->timeLimit-(time(NULL)-t1));
    copyArray(solution->exams,temp->temporarySolution,solution->E);
    print_Sol(argv[1], solution);


     // Take as input the solution and the max time of execution

	localSearch(solution,solution->timeLimit-(time(NULL)-t1));
    localSwap(solution, solution->timeLimit-(time(NULL)-t1));
	localSearch(solution,solution->timeLimit-(time(NULL)-t1));
    print_Sol(argv[1], solution);


	int round=0;
    while (time(NULL)-t1<solution->timeLimit){
        simulateAnnealingSearch(solution,round++,solution->timeLimit-(time(NULL)-t1));
	    localSearch(solution,solution->timeLimit-(time(NULL)-t1));
        localSwap(solution,solution->timeLimit-(time(NULL)-t1));
        print_Sol(argv[1], solution);
    }

	print_Sol(argv[1],solution);
    freeTempSol(temp);
    STfree(solution->tab);
    GraphFree(solution->g);
    free(solution->exams);

    t2 = time(0);
    printf("Finished in %d seconds\n", (int) (t2-t1));
    return 0;
}
