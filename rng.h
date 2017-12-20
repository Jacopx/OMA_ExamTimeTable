/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef OMA_EXAMTIMETABLE_RNG_H
#define OMA_EXAMTIMETABLE_RNG_H

#include <time.h>
#include <stdlib.h>

#define randomize() (srand(time(NULL)))
#define random( x, y ) ((rand()%((y)-(x)+1))+(x))
#define fate(x) ((random(0,9999)<(x))?1:0)

#endif //OMA_EXAMTIMETABLE_RNG_H
