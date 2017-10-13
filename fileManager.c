/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Function that opens the filename pointed to, by filename using reading mode
 *  PARAMETERS: 
 *    > filename: C string containing the name of file to be opened 
 *  OUTPUT: 
 *    > This function returns a FILE pointer. Otherwise, NULL is returned.
 */

FILE* file_Open(char *filename,char *mode)
{
    return fopen(filename,mode);
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
 *        OPTION 2: An array sPe_array of structures. Each structures is defined by two attributes: exam ID and the 
 *                  # students enrlolled
 *        OPTION 3: A data structure that brings benefits to performances based on developed algorithm
 */ 
 
int read_Exm(FILE *instanceExm, int *dataStructure)
{
    int exmId, sEnr;
    if(instanceExm)
    {
        while(fscanf(instanceExm,"%d %d",&exmId,&sEnr) != EOF)
        {
            if(!ferror(instanceExm))
            {
                /*TO-DO: fill data structure*/
            }
            else
            {
                return 0;
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }  
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
 *        OPTION 1: An array ePs_array of structures. Each structures is defined by two attributes: student ID and the 
 *                  array of eaxam IDs in which the student is enrlolled
 *        OPTION 2: A data structure that brings benefits to performances based on developed algorithm
 */ 
 
int read_Stu(FILE *instanceStu, int *dataStructure, int digits)
{
    char sId[digits]; /*TO-DO: decide how to define size of this field*/
    int exmId; 
    if(instanceStu)
    {
        while(fscanf(instanceStu,"%s %d",sId,&exmId) != EOF)
        {
            if(!ferror(instanceStu))
            {
                /*TO-DO: fill data structure*/
            }
            else
            {
                return 0;
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }  
}
