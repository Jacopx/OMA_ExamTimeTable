#ifndef OMA_EXAMTIMETABLE_ST_H
#define OMA_EXAMTIMETABLE_ST_H

typedef struct ST *st;
typedef struct bstnode *bstlink;

st STinit(int n);
void STinsert(st tab, char *key);
int STsearchid(st tab, char *nome);
char* STdisplay(st tab, int i);
void STfree(st tab);

#endif //OMA_EXAMTIMETABLE_ST_H
