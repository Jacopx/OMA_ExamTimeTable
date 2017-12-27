/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef OMA_EXAMTIMETABLE_LOCAL_H
#define OMA_EXAMTIMETABLE_LOCAL_H

#include "utils.h"
#include "verbose.h"

void localSearch(dataStructure * sol,int maxTime);
void localSearchTemp(dataStructure * sol,int* temp,int maxTime);
void localSearch2Temp(dataStructure * sol,int* temp,int maxTime);
void localSwap(dataStructure* solution, int maxTime);
void localSwapTemp(dataStructure* solution,int*, int maxTime);
void swapExam(int *sol,int a,int b);
void swapExam2(int *sol,int a,int b, int c);
#endif //OMA_EXAMTIMETABLE_LOCAL_H
