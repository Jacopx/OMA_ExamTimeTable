/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef OMA_EXAMTIMETABLE_EDGE_H
#define OMA_EXAMTIMETABLE_EDGE_H

typedef struct Edge{
    int v, w, wt;
} edge;

edge EDGEcreate(int v, int w, int wt);

#endif //OMA_EXAMTIMETABLE_EDGE_H
