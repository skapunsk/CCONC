#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

long int N; // dimensão do vetor
float *vetor; // vetor principal 
long int nthreads; // numero de threads utilizadas no processamento

typedef struct{
   long int id;
   float min; 
   float max; 
} tArgs;

/*Dividindo a Tarefa intercalando as threads pulando nthreads posicoes ate a proxima posiçao a ser checada no vetor dai são feitos
os 2 testes para checar se ela é maior ou menor que todas as outras nesse bloco tid+n*nthreads */
void *tarefa(void *arg) {
   tArgs *args = (tArgs*) arg;
   long int id = (long int) args->id;
   for(long int i = id*(N/nthreads); i<id*(N/nthreads)+(N/nthreads); i++){
       if(vetor[i] < args->min){
            args->min = vetor[i];
       }
       if(vetor[i] > args->max){
            args->max = vetor[i];
       }
   }
   pthread_exit((void *) args);
}

int main(int argc, char* argv[]) {
    pthread_t *tid;
    tArgs *args;
    tArgs *resT; //guarda o max e min naquela execucao da thread
    double comeco, final, delta;
    float minC; // menor numero no vetor calculado concorrentemente
    float maxC; // maior número no vetor calculado concorrentemente
    float minS; // menor numero no vetor calculado sequencialmente
    float maxS; // maior numero no vetor calculado sequencialmente

    if(argc<3) {
      printf("Digite: %s <tamanho do vetor> <numero de threads>\n", argv[0]);
      return 1;
    }
    N = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > N) nthreads=N;
   
    vetor = (float *) malloc(sizeof(float) * N);
    if (vetor == NULL) {printf("ERRO--malloc\n"); return 2;}

    
    // Processo de achar sequencialmente o menor e o maior numero no vetor
    GET_TIME(comeco);
    for(long int i = 0; i < N; i++){
        if(i == 0){
            minS = vetor[i];
            maxS = vetor[i];
        }
        if(vetor[i] < minS){
            minS = vetor[i];
        }
        if(vetor[i] > maxS){
            maxS = vetor[i];
        }
    }
    GET_TIME(final);
    delta = final - comeco;
    printf("Tempo Sequencial: %lf\n", delta);

    GET_TIME(comeco);
    //alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}

    //criacao das threads
    for(long int i=0; i<nthreads; i++) {
        (args+i)->id = i;
        (args+i)->min = vetor[i];
        (args+i)->max = vetor[i];
        if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    } 

    //espera pelo termino da threads
    for(long int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), (void**)&resT);
        if(i == 0){
            minC = resT->min;
            maxC = resT->min;
        }else{
        if(minC > resT->min){
            minC = resT->min;
        }
        if(maxC < resT->max){
            maxC = resT->max;
        }
        }
    }
    GET_TIME(final);   
    delta = final - comeco;
    printf("Tempo Concorrente: %lf\n", delta);

    /*testa a corretude 
    if(maxC == maxS && minC == minS){
        printf("Tudo certo!");
    }
    */
    free(vetor);
    free(tid);
    free(args);
    return 0;
}