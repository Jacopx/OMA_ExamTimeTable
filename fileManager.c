/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileManager.h"
#include "utils.h"

typedef struct {
    int num;
    char Ids[10];
} orderedExams;

/*
 * Function that opens the filename pointed to, by filename using reading mode
 *  PARAMETERS: 
 *    > filename: C string containing the name of file to be opened 
 *  OUTPUT: 
 *    > This function returns a FILE pointer. Otherwise, NULL is returned.
 */

FILE* file_Open(char *filename, char *mode)
{
    FILE *fp = fopen(filename,mode);
    if (fp == NULL) {
        printf("Error opening file: %s", filename);
        exit(1);
    }
    return fp;
}


/*
 * Function that reads formatted input by .exm file that defines the total number of students enrolled per exam
 *  PARAMETERS: 
 *    > instanceExm: pointer to .exm file that identifies the stream
 *    > dataStructure: data structures in which data will be stored 
 *  OUTPUT: 
 *    > This function returns 1 if reading is carry out. Otherwise, 0 is returned.
 */
/* TO-DO: Define data structures where data will be stored
 *        OPTION 1: Based on instance examples, exams have sequential ID (from 0 to...) so this data could be stored
 *                  in a simnple array (sPe_array: Student per exam) where indices rappresent the exam ID, and i-element
 *                  the #students enrolled
 *        OPTION 2: An array sPe_array of structures. Each structure is defined by two attributes: exam ID and the
 *                  # students enrlolled
 *        OPTION 3: A data structure that brings benefits to performances based on developed algorithm
 */

int cmpFunction (const void *a, const void *b) {
    orderedExams *o1 = *(orderedExams**)a;
    orderedExams *o2 = *(orderedExams**)b;
    return o2->num - o1->num;
}

int cmpFunctionForId (const void *a, const void *b) {
    orderedExams *o1 = *(orderedExams**)a;
    orderedExams *o2 = *(orderedExams**)b;

    if (strcmp(o1->Ids, o2->Ids) > 0) return 1;
    else if (strcmp(o1->Ids, o2->Ids) < 0) return -1;
    return 0;
}

char** getOrderedData (int n, FILE *fp) {
    int i;
    char **ret, str1[10], str2[10];
    orderedExams **s = malloc(n * sizeof(orderedExams*));
    ret = malloc(n * sizeof(char*));
    for (i = 0; i < n; ++i) s[i] = malloc(sizeof(orderedExams));
    for (i = 0; i < n; ++i) ret[i] = malloc(sizeof(10 * sizeof(char)));

    i = 0;
    while(fscanf(fp, "%s%s", str1, str2) != EOF) {
        strcpy(s[i]->Ids, str1);
        s[i]->num = atoi(str2);
        i++;
    }

    // sort the ids
    qsort(s, (unsigned int)n, sizeof(orderedExams*), cmpFunction);

    for (i = 0; i < n; ++i) {
        strcpy(ret[i], s[i]->Ids);
        free(s[i]);
    }
    free(s);
    return ret;
}

void read_Exm(char *filename, dataStructure *solution)
{
    FILE *fp;
    int nline = 0, i;
    char line[100];
    
    // read file to know how many exams there are
    fp = file_Open(filename, "r");
    while (fgets(line, 100, fp) != NULL) nline++;
    // nline--;
    fclose(fp);
    printf("nline %d\n", nline);
    
    // fill the solution data structure
    solution->exams = malloc(nline * sizeof(int));
    solution->E = nline;
    solution->g = GraphInit(nline);
    solution->tab = STinit(nline);
    
    // read the data ordered by decreasing number of enrolled student
    fp = file_Open(filename, "r");
    char **ids = getOrderedData(nline, fp);
    fclose(fp);

    // insert data into the ST structure
    for (i = 0; i < nline; ++i) {
        STinsert(solution->tab, ids[i]);
        free(ids[i]);
    }
    free(ids);
}
   

/*
 * Function that reads formatted input by .slo file that defines the lenght of the examination period.
 *  PARAMETERS: 
 *    > instanceSlo: pointer to .slo file that identifies the stream
 *    > tMax: pointer to variable in which number of slots will be stored 
 *  OUTPUT: 
 *    > This function returns 1 if reading is carry out. Otherwise, 0 is returned.
 */
 
int read_Slo(FILE *instanceSlo, int *tMax)
{
    if(instanceSlo)
    {
        fscanf(instanceSlo,"%d",tMax);
        if(ferror(instanceSlo))
        {
                return 0;
        }
        return 1;
    }
    else
    {
        return 0;
    }  
}


/*
 * Function that reads formatted input by .stu file that defines the exams in which each student is enrolled.
 *  PARAMETERS: 
 *    > instanceStu: pointer to .exm file that identifies the stream
 *    > dataStructure: data structures in which data will be stored 
 *    > digits: the number of digits for student ID (calculated for excess before from the sum of # of students per exam).
        It could set a fixed size (for ex. 20), but there is a possibility to understimate this value. 
 *  OUTPUT: 
 *    > This function returns 1 if reading is carry out. Otherwise, 0 is returned.
 */
/* TO-DO: Define data structures where data will be stored
 *        OPTION 1: An array ePs_array of structures. Each structure is defined by two attributes: student ID and the 
 *                  array of eaxam IDs in which the student is enrlolled
 *        OPTION 2: A data structure that brings benefits to performances based on developed algorithm
 */ 
 
int read_Stu(FILE *instanceStu, dataStructure *solution)
{
    int i, n, id1, id2, count = 0;
    char sId[10], currentSIs[10], exmId[10], **enrExams;

    enrExams = malloc(solution->E * sizeof(char*));
    for (i = 0; i < solution->E; ++i) enrExams[i] = malloc(10 * sizeof(char));

    strcpy(currentSIs, "");
    n = 0;
    while(fscanf(instanceStu,"%s%s",sId,exmId) != EOF) {
        count++;
        if (strcmp(sId, currentSIs) == 0) {
            strcpy(enrExams[n++], exmId);
        } else {
            while (n > 1) {
                for (i = 0; i < n - 1; ++i) {
                    id1 = STsearchid(solution->tab, enrExams[i]);
                    id2 = STsearchid(solution->tab, enrExams[n - 1]);
                    GraphInsertE(solution->g, EDGEcreate(id1, id2, 1));
                }
                n--;
            }

            strcpy(currentSIs, sId);
            n = 0;
            strcpy(enrExams[n++], exmId);
        }
    }
    while (n > 1) {
        for (i = 0; i < n - 1; ++i)
            GraphInsertE(solution->g, EDGEcreate(STsearchid(solution->tab, enrExams[i]), STsearchid(solution->tab, enrExams[n - 1]), 1));
        n--;
    }
    printf("count -> %d <-\n", count);
}


/*
 * Function that sends formatted output to result stream.
 *  PARAMETERS: 
 *    > filename: C string containing the name of instance to be opened
 *    > result: data structures in which results will be stored. 
 *  OUTPUT: 
 *    > This function returns 1 if printing is carry out. Otherwise, 0 is returned.
 */
/* TO-DO: Define data structures where result will be stored
 *        OPTION 1: Based on instance examples, exams have sequential ID (from 0 to...) so results could be stored
 *                  in a simnple array (slotPe_array: Slot per exam) where indices rappresent the exam ID, and i-element
 *                  the slot number assigned to that exam
 *        OPTION 2: An array slotPe_array of structures. Each structure is defined by two attributes: exam ID and the 
 *                  slot number assigned to it
 *        OPTION 3: A data structure that brings benefits to performances based on developed algorithm
 */

int print_Sol(char *instanceName, dataStructure *solution)
{
    FILE* f;
    int i;
    orderedExams **e = malloc(solution->E * sizeof(orderedExams*));
    for (i = 0; i < solution->E; ++i) e[i] = malloc(sizeof(orderedExams));

    for (i = 0; i < solution->E; ++i) {
        e[i]->num = solution->exams[i];
        strcpy(e[i]->Ids, STdisplay(solution->tab, i));
    }

    printf("solution:\n");
    for (i = 0; i < solution->E; ++i) printf("%d ", solution->exams[i]);
    printf("\n");

    qsort(e, (unsigned int)solution->E, sizeof(orderedExams*), cmpFunctionForId);

    /* TO-DO: Define group number YY */
    // strcat(instanceName,"_OMAMZ_groupYY.sol");
    if(f=fopen(instanceName,"w"))
    {
        /*TO-DO: print results*/

        fprintf(f, "%s %d", e[0]->Ids, e[0]->num);
        for (i = 1; i < solution->E; ++i) fprintf(f, "\n%s %d", e[i]->Ids, e[i]->num);
        fclose(f);
    }

    for (i = 0; i < solution->E; ++i) free(e[i]);
    free(e);
}
