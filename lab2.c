#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

int *mat1; // matriz multiplicando
int *mat2; // matriz multplicadora
int *saidaConc; // matriz de saida
int nthreads; // numero de threads

typedef struct{
    int id; 
    int dim; 
}tArgs;

//funcao executada pelas threads 
void * tarefa(void *arg){
    tArgs *args = (tArgs*) arg;
    for(int i=args->id;i<args->dim;i+=nthreads){
        for(int j = 0; j < args->dim; j++){
            for(int k = 0; k < args->dim; k++){
                saidaConc[i*(args->dim)+j] += mat1[i*(args->dim)+k]*mat2[k*(args->dim)+j];
            }
        }
    }
    pthread_exit(NULL);
}   

    
int main(int argc, char* argv[]){
    int dim; 
    int *saidaSeq; 
    pthread_t *tid;
    tArgs *args;
    double inicio, fim, delta; 

    //leitura e avaliação dos parametros de entrada
    if(argc<3){
        printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    
    //alocacao de memoria para as estruturas de dados
    mat1 = malloc(sizeof(int)*dim*dim);
    if(mat1 == NULL){
        printf("ERRO -- malloc");
        return 2;
    };
    mat2 = malloc(sizeof(int)*dim*dim);
    if(mat2 == NULL){
        printf("ERRO -- malloc)");
        return 2;
    };
    saidaSeq = malloc(sizeof(int)*dim*dim);
    if(saidaSeq == NULL){
        printf("ERRO -- malloc");
        return 2;
    };
    saidaConc = malloc(sizeof(int)*dim*dim);
    if(saidaConc == NULL){
        printf("ERRO -- malloc");
        return 2;
    };

    // inicializacao das matrizes
    srand(time(NULL));
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            mat1[i*dim+j] = (rand() % 10); 
            mat2[i*dim+j] = (rand() % 10);
            saidaSeq[i*dim+j] = 0;
            saidaConc[i*dim+j] = 0;
        }
    }
    // resolucao sequencial
    GET_TIME(inicio);
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            for(int k=0; k<dim; k++){
                saidaSeq[i*dim+j] += mat1[i*dim+k]*mat2[k*dim+j];
            }
        }
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo Sequencial: %lf \n", delta);

    // resolucao concorrente
    GET_TIME(inicio);
    // alocacao das threads
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid == NULL){
        puts("ERRO -- malloc");
        return 2;
    }
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args == NULL){
        puts("ERRO -- malloc");
        return 2;
    }

    // criacao das threads
    for(int i=0;i<nthreads;i++){
        (args+i)->id = i;
        (args+i)->dim = dim;
        if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
            puts("ERRO -- pthread_create");
            return 3;
        }
    }

    // espera pelas threads terminarem
    for(int i=0;i<nthreads;i++){
        pthread_join(*(tid+i), NULL);
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo Concorrente: %lf \n", delta);

    /* conferimento dos resultados das multiplicacoes concorrentes e sequencial

    for(int i=0;i<dim*dim;i++){
        if(saidaSeq[i] != saidaConc[i]){
                printf("ERRO --- saida errada no indice: %d ",i);
            return 4;
        }
    } */

    //liberação de memoria
    free(mat1);
    free(mat2);
    free(saidaSeq);
    free(saidaConc);
    free(args);
    free(tid);
    return 0;
}
