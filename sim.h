#ifndef _SIM_H_
#define _SIM_H_

#include <stdint.h>
#include <math.h>

struct req{
    int device;
    int length;
};

typedef struct dev{
    int irq;
    int pedido;
    int lat;
    float transm;
    int tipo;
    int complete;
}DEV;

typedef struct nodo{
    DEV info;
    struct nodo *prox;
}NODO;

typedef struct list{
        NODO *info;
        int qtd;
}LIST;
/*

*/

int sim(struct req * requests, int num_requests);

LIST* CreateList(LIST *p);
int Add(LIST *p, DEV a);
int deleta(LIST *p);
void imprimeLista(LIST *p);

DEV config(int requests, DEV new, float aux);
float arredondar(float x);

#endif // _SIM_H_
