/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>

#ifndef OMA_EXAMTIMETABLE_FILEMANAGER_H
#define OMA_EXAMTIMETABLE_FILEMANAGER_H

FILE* file_Open(char *filename,char *mode);
int read_Exm(FILE *instanceExm, int *dataStructure);
int read_Slo(FILE *instanceSlo, int *tMax);
int read_Stu(FILE *instanceStu, int *dataStructure, int digits);
int print_Sol(char *instanceName, int *result);

#endif //OMA_EXAMTIMETABLE_FILEMANAGER_H
