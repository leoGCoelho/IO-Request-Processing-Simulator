#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sim.h"


// Main
int sim(struct req * requests, int num_requests){
    if((num_requests <= 0) || (requests == NULL)) return -1;

    int i, m, result=0, ctrl[7], ready=0;
    float aux;

    LIST *device[7];
    DEV new;

    for(i=0; i<7; i++){
        device[i] = (LIST *) malloc(sizeof(LIST));
        device[i] = CreateList(device[i]);
        ctrl[i] = 0;
    }

    if(num_requests == 1){
        aux = (float)requests->length;
        new = config(requests->device, new, aux);
        result = new.pedido + new.lat + new.transm; // Resultado
        if(new.tipo != 3) result = result +2; // interrupcao
    }

    else{

        for(i=0; i<num_requests; i++){
            aux = (float)requests[i].length;
            new = config(requests[i].device, new, aux);
            if((Add(device[new.irq-1], new)) == 0) return -1;
        }

        /*for(i=0; i<7; i++){new.irq-1
            imprimeLista(device[i]);
        }*/

        while(ready != num_requests){
            for(i=0; i<7; i++){
                if(device[i]->qtd != 0){
                    if(i==0){
                        m = device[i]->info->prox->info.lat;
                    }
                    else{
                        if(m < device[i]->info->prox->info.lat)
                            m = device[i]->info->prox->info.lat;
                        else
                            m=m;
                    }

                    if(ctrl[i] == 0){

                        if(m > device[i]->info->prox->info.pedido + device[i]->info->prox->info.lat + device[i]->info->prox->info.transm){
                            device[i]->info->prox->info.pedido = 0;
                            device[i]->info->prox->info.lat = 0;
                            device[i]->info->prox->info.transm = 0;
                            result = (device[i]->info->prox->info.tipo != 3) ? result + 2 : result;
                            if(!(deleta(device[i]))) return -1;
                            ready = +1;
                        }
                        else{
                            result = result + device[i]->info->prox->info.pedido;
                            ctrl[i] = 1;
                        }

                    }
                    else{
                        if(device[i]->info->prox->info.lat <= 0){
                            if(device[i]->info->prox->info.transm <= 0){
                                ctrl[i] = 0;
                                if(device[i]->info->prox->info.tipo != 3) result = result+2;
                                if(!(deleta(device[i]))) return -1;
                                ready = ready +1;
                            }
                            else{
                                device[i]->info->prox->info.transm = device[i]->info->prox->info.transm -1;
                                result = result +1;
                            }
                        }

                        else{
                            result = result +1;
                            device[i]->info->prox->info.lat = device[i]->info->prox->info.lat-1;
                        }
                    }
                }
            }

        }
        result = result-2;

    }

    return result;
}


// Lista
LIST* CreateList(LIST *p){
    NODO *n;
    n = (NODO *) malloc(sizeof(NODO)); //testa se é possivel alocar espaço para a lista
    n->prox = NULL;
    p->info = n;
    p->qtd = 0;

    return p;
}

int Add(LIST *p, DEV a){
    NODO *x = p->info->prox;
    NODO *new = (NODO *) malloc(sizeof(NODO));

    new->info = a;

    new->prox = x;
    x = new;

    p->info->prox = x;
    p->qtd = p->qtd + 1;
    return 1;

}

int deleta(LIST *p){
	NODO *n = p->info->prox;

    if(n == NULL) return 0;
	p->info->prox = n->prox;
    p->qtd = p->qtd-1;
    free(n);
    return 1;
}

void imprimeLista(LIST *p){
	if(p->qtd != 0){
		NODO *n = p->info->prox;

		while(n != NULL){
			printf("%d ", n->info.irq);
			n = n->prox;
		}
	}
}


// Outras funcoes
float arredondar(float x){
    int y;
    float teste;

    y = x;
    teste = x - y;
    return (teste > 0) ? x+1 : x;
}

DEV config(int requests, DEV new, float aux){
    // | IRQ | DISPOSITIVO  | PEDIDO (ciclos) | LATÊNCIA | TRANSMISSÃO |    TIPO     |
    // |  1  | Disco rígido |       6         |   5 ms   | 80 MiB/s    |    DMA      |
    if(requests == 1){
        new.pedido = 6;
        new.lat = 5 * pow(10,6);
        new.transm = (aux / 83886080)  * pow(10,9); // ns
        new.tipo = 1; // DMA
    }
    // |  2  | Disco rígido |       6         |   7 ms   | 60 MiB/s    |    DMA      |
    if(requests == 2){
        new.pedido = 6;
        new.lat = 7 * pow(10,6);
        new.transm = (aux / 62914560)  * pow(10,9); // ns
        new.tipo = 1; // DMA
    }
    // |  3  |   USB        |       6         |   1 ms   | 40 MiB/s    |    DMA      |
    if(requests == 3){
        new.pedido = 6;
        new.lat = 1 * pow(10,6);
        new.transm = (aux / 41943040)  * pow(10,9); // ns
        new.tipo = 1; // DMA
    }
    // |  4  |   USB        |       6         |   1 ms   | 20 MiB/s    |    DMA      |
    if(requests == 4){
        new.pedido = 6;
        new.lat = 1 * pow(10,6);
        new.transm = (aux / 20971520)  * pow(10,9); // ns
        new.tipo = 1; // DMA
    }
    // |  5  |   USB        |       6         |   1 ms   | 20 MiB/s    | Interrupção |
    if(requests == 5){
        new.pedido = 6;
        new.lat = 1* pow(10,6);
        new.transm = (aux / 20971520)  * pow(10,9); // ns
        new.tipo = 2; // INTERRUPCAO
    }
    // |  6  |   Teclado    |       4         |  100 ns  | 10 KiB/s    | Interrupção |
    if(requests == 6){
        new.pedido = 4;
        new.lat = 100;
        new.transm = (aux / 10240)  * pow(10,9); // ns
        new.tipo = 2; // INTERRUPCAO
    }
    // |  7  | Porta serial |       6         |  10 ns   | 1 KiB/s     |   Polling   |
    if(requests == 7){
        new.pedido = 6;
        new.lat = 10;
        new.transm = (aux / 1024)  * pow(10,9); // ns
        new.tipo = 3; // POLLING
    }

    if(requests == 1 || requests == 2) new.irq = 1;
    if(requests == 3 || requests == 4 || requests == 5) new.irq = 3;
    else new.irq = requests;
    
    new.transm = arredondar(new.transm);
    new.complete = 0;

    return new;
}
