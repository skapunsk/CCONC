#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define VETSIZE 10000
#define NTHREADS 2

int vetor[VETSIZE];

/* A função Quadrado divide o trabalho, de elevar ao quadrado todos numeros no vetor, entre NTHREADS.
    No caso de 2 threads usadas, a thread de indice 0 elevara todos os números em casas par
    e a segunda thread, de indice 1, elevara todos os numeros em casas impar do vetor
*/
void *Quadrado (void * arg) {
    int idThread = * (int*) arg;
    for(int i = idThread; i<VETSIZE; i = i+NTHREADS){
        vetor[i] = vetor[i] * vetor[i];
    }
    pthread_exit(NULL);
}

int main (){
    pthread_t tid_sistema[NTHREADS];
    int thread;
    int tid_local[NTHREADS];

    //int teste[VETSIZE];

    // Inicializacao do vetor
    for (int i = 0; i<VETSIZE; i++){
        vetor[i] = 5;
    }

    /*for (int i = 0; i<VETSIZE; i++){
        teste[i] = vetor[i];
    }*/

    for(thread=0; thread<NTHREADS; thread++) {
        tid_local[thread] = thread;
        pthread_create(&tid_sistema[thread], NULL, Quadrado, (void*) &tid_local[thread]);
    }

    for (thread=0; thread<NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }

    /* @Testes 
    Caso da inicialização feita em que todas posicoes tem o numero 5:
    for(int i = 0;  i<VETSIZE;  i++)    {
        if(vetor[i] != 25 ){
            printf("--ERRO: Valor final em %d nao é o quadrado do valor inicial nessa posicao",i);
            return 0;
        }
    }
    Caso geral usando um vetor para teste com os mesmos valores do vetor na inicializ :
    for(int i = 0; i<VETSIZE; i++){
        if(vetor[i] != teste[i] * teste[i]){
            printf("--ERRO: Valor final em %d nao é o quadrado do valor inicial nessa posicao",i);
            return 0;
        }
    }
    */
    pthread_exit(NULL);
}
