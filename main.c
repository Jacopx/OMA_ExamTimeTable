/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include "fileManager.h"
#include <time.h>
#include "local.h"
#include "rng.h"

int main(int argc, char **argv) {
    dataStructure *solution[10];
    TempSol *temp = NULL;
    static time_t t1, t2;
    int i;
    randomize();

    for(i=0;i<10;i++)
        solution[i]=malloc(sizeof(dataStructure));
    if (argc == 4) solution[0]->timeLimit = atoi(argv[argc - 1]);
    else solution[0]->timeLimit = 0;

    t1 = time(0);

    read_Slo(argv[1], solution[0]);
    read_Exm(argv[1], solution[0]);
    read_Stu(argv[1], solution[0]);

    sortBasedOnEdges(solution[0]);
    read_Stu(argv[1], solution[0]);

    temp = newTempSol(solution[0]);
    findFeasibleGreedyCi(solution[0], temp);
    findFeasibleSolution(solution[0], temp);
    copyArray(solution[0]->exams,temp->temporarySolution,solution[0]->E);
    print_Sol(argv[1], solution[0]);
    free(temp);

    localSearch(solution[0], 200);

    /*  Take as input the solution and the max time of execution
     * TODO IMPLEMENT MAX EXECUTION TIME*/
    localSwap(solution[0], 200);
    print_Sol(argv[1], solution[0]);

    ////choose how to use it
    /*
    while (time(NULL)-t1<180){
        simulateAnnealingSearch(solution[0], 10);

        //localSearch(solution[0], 30);
        //localSwap(solution[0]);
        print_Sol(argv[1], solution[0]);
    }
    */

    freeTempSol(temp);
    STfree(solution[0]->tab);
    GraphFree(solution[0]->g);
    free(solution[0]->exams);
    //free(solution);

    t2 = time(0);
    printf("Finished in %d seconds\n", (int) (t2-t1));
    return 0;
}
