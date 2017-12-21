//
// Created by munfo on 21/12/2017.
//

#ifndef OMA_EXAMTIMETABLE_GREEDYSLOTS_H
#define OMA_EXAMTIMETABLE_GREEDYSLOTS_H

#include "utils.h"

void greedySlots(dataStructure* sol,int maxTime);
void greedySlotsOrdered(dataStructure *sol, int maxTime);
void swapSlotsGreedy(const dataStructure *sol,int *temp, int from, int to);
void backtrackGreedyslots(const dataStructure *sol, int *temp,int to);
void orderSlots(dataStructure *sol,int *order);
#endif //OMA_EXAMTIMETABLE_GREEDYSLOTS_H
