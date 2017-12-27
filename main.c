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
#include "greedySlots.h"

int main(int argc, char **argv) {
    dataStructure *solution;
    static time_t t1, t2;
    randomize();
    char *instanceName = argv[1];

    solution=malloc(sizeof(dataStructure));
    if (argc == 4) solution->timeLimit = atoi(argv[argc - 1]);
    else solution->timeLimit = 0;

    t1 = time(0);

    read_Slo(instanceName, solution);
    read_Exm(instanceName, solution);
    read_Stu(instanceName, solution);

    sortBasedOnEdges(solution);
    read_Stu(instanceName, solution);

    TempSol *temp = newTempSol(solution);
    findFeasibleGreedyCi(solution, temp);
    findFeasibleSolution(solution, temp,solution->timeLimit-(time(NULL)-t1));
    copyArray(solution->exams,temp->temporarySolution,solution->E);
    print_Sol(instanceName, solution);

    // Take as input the solution and the max time of execution
	//------>1//localSearch(solution,solution->timeLimit-(time(NULL)-t1));
    localSwap(solution, solution->timeLimit-(time(NULL)-t1));
	//greedySlotsOrdered(solution,solution->timeLimit-(time(NULL)-t1)); TODO broken
	print_Sol(instanceName, solution);
	printf("At %d seconds",(int)time(NULL)-t1);
	localSearch(solution,solution->timeLimit-(time(NULL)-t1));
    print_Sol(instanceName, solution);
    printf("At %d seconds",(int)time(NULL)-t1);
	int round=0;
    while (time(NULL)-t1<solution->timeLimit){
        simulateAnnealingSearch(solution,round++,solution->timeLimit-(time(NULL)-t1));
	    //localSearch(solution,solution->timeLimit-(time(NULL)-t1));
        greedySlotsShuffle(solution,solution->timeLimit-(time(NULL)-t1));
        localSwap(solution, solution->timeLimit-(time(NULL)-t1));
        localSearch(solution,solution->timeLimit-(time(NULL)-t1));
        print_Sol(instanceName, solution);
        printf("At %d seconds",(int)time(NULL)-t1);
    }

	print_Sol(instanceName,solution);
    freeTempSol(temp);
    STfree(solution->tab);
    GraphFree(solution->g);
    free(solution->exams);

    t2 = time(0);
    printf("\nFinished in %d seconds", (int) (t2-t1));
    return 0;
}
