/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Edge.h"

#ifndef OMA_EXAMTIMETABLE_ADTGRAPH_H
#define OMA_EXAMTIMETABLE_ADTGRAPH_H

typedef struct graph *Graph;

Graph GraphInit (int V);
void GraphInsertE (Graph g, edge e);
void GraphRemoveE (Graph g, edge e);
void GraphFree (Graph g);

#endif //OMA_EXAMTIMETABLE_ADTGRAPH_H
