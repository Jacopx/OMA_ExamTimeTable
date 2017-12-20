/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *          OMA Project of Jacopo Maggio, Stefano Munna, Jacopo Nasi, Andrea Santu and Marco Torlaschi               *
 *                      Repository available on https://github.com/Jacopx/OMA_ExamTimeTable                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ST.h"

struct bstnode{
    char *key; // key of the node
    int id; // index into the direct access array
    bstlink l, r; // left and right node
};

struct ST{
    bstlink h, z; /*z nodo fittizzio*/
    char **v; // vettore ad accesso diretto
    int bstsize, n; /*n elementi correnti*/
};

bstlink bnodesetvoid() {
    bstlink t = malloc(sizeof(*t));
    t->key = NULL;
    t->id = -1;
    t->l = t->r = NULL;
    return t;
}

bstlink newnode(char *key, bstlink l, bstlink r, int i) {
    bstlink nl = malloc(sizeof(*nl));
    nl->key = malloc(strlen(key) * sizeof(char));
    strcpy(nl->key, key);
    nl->l = l;
    nl->r = r;
    nl->id = i;
    return nl;
}

st STinit(int n) {
    int i;
    st bst = malloc(sizeof(*bst));
    bst->z = bnodesetvoid();
    bst->h = bst->z;
    bst->v = malloc(n*sizeof(char*));
    for(i=0; i<n; i++)
        bst->v[i] = malloc(10 * sizeof(char));
    bst->bstsize = n;
    bst->n = 0;
    return bst;
}

bstlink insertR(bstlink h, char *key, bstlink z, int i) {
    if(h == z)
        return newnode(key, z, z, i);
    if(strcmp(key, h->key) > 0)
        h->r=insertR(h->r, key, z, i);
    else
        h->l=insertR(h->l, key, z, i);
    return h;
}

void STinsert(st tab, char *key) {
    if (tab->n + 1 > tab->bstsize) printf("Error in bstsize\n");
    int i = tab->n++;
    strcpy(tab->v[i], key);
    tab->h=insertR(tab->h, key, tab->z, i);
}

int searchR(bstlink h, bstlink z, char *nome) {
    if(h==z)
        return -1;
    if(strcmp(nome, h->key) == 0)
        return h->id;

    if(strcmp(nome, h->key)>0)
        return searchR(h->r, z, nome);
    else
        return searchR(h->l, z, nome);
}

int STsearchid(st tab, char *nome) {
    return searchR(tab->h, tab->z, nome);
}

char* STdisplay(st tab, int i) {
    return tab->v[i];
}


void freeR(bstlink h, bstlink z) {
    if(h==z) return;
    freeR(h->l, z);
    freeR(h->r, z);
//    free(h->key);
    free(h);
}

void STfree(st tab) {
    int i;
    freeR(tab->h, tab->z);
    free(tab->z);
    for (i = 0; i < tab->bstsize; ++i) free(tab->v[i]);
    free(tab->v);
    free(tab);
}
