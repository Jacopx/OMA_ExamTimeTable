/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include "utils.h"
#include "verbose.h"

#ifndef OMA_EXAMTIMETABLE_FILEMANAGER_H
#define OMA_EXAMTIMETABLE_FILEMANAGER_H

FILE* file_Open(char *filename,char *mode);
void read_Exm(char *filename, dataStructure *solution);
int read_Slo(char *filename, dataStructure *solution);
int read_Stu(char *filename, dataStructure *solution);
int print_Sol(char *filename, dataStructure *solution);
void sortBasedOnEdges (dataStructure *solution);


#endif //OMA_EXAMTIMETABLE_FILEMANAGER_H
