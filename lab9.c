#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int contador = 0; // contador de quantas mensagens foram printadas na tela após a introdução
sem_t em;     // semaforo para exclusao mutua entre as threads
sem_t condt5, condt1;     //semaforos para sincronizar a ordem de execucao das threads

// Tarefa que a Thread 5 ira realizar
void *Greet (void *arg) {
    printf("Seja bem-vindo!\n");
    sem_post(&condt5); sem_post(&condt5); sem_post(&condt5); // liberando as 3 threads de "small talk"
    pthread_exit(NULL);
}

// Tarefa que a Thread 2 ira realizar 
void *ST () {
    
    sem_wait(&condt5);
    printf("Fique a vontade.\n");
    sem_wait(&em); //entrada na secao critica
    contador++;
    if(contador >= 3){
        sem_post(&condt1); // liberando a thread 1 de despedida
    }
    sem_post(&em); //saida da secao critica
    pthread_exit(NULL);
}

// Tarefa que a Thread 3 ira realizar 
void *ST1 () {

    sem_wait(&condt5);
    printf("Sente-se por favor.\n");
    sem_wait(&em); //entrada na secao critica
    contador++;
    if(contador >= 3){
        sem_post(&condt1); // liberando a thread 1 de despedida
    }
    sem_post(&em); //saida da secao critica
    pthread_exit(NULL);
}

// Tarefa que a Thread 4 ira realizar 
void *ST2 () {

    sem_wait(&condt5);
    printf("Aceita um copo d'agua?\n");
    sem_wait(&em); //entrada na secao critica
    contador++;
    if(contador >= 3){
        sem_post(&condt1); // liberando a thread 1 de despedida
    }
    sem_post(&em); //saida da secao critica
    pthread_exit(NULL);
}

// Tarefa que a Thread 1 ira realizar 
void *GB () {
    sem_wait(&condt1);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}

int main (int argc, char *argv[]){
    pthread_t t[5];
    //inicia o semaforo binario
    sem_init(&em, 0, 1);
    //inicia os semaforos
    sem_init(&condt5, 0, 0);
    sem_init(&condt1, 0, 0);
    
    // Cria as threads 
    pthread_create(&t[0],NULL,GB,NULL);
    pthread_create(&t[1],NULL,ST,NULL);
    pthread_create(&t[2],NULL,ST1,NULL);
    pthread_create(&t[3],NULL,ST2,NULL);
    pthread_create(&t[4],NULL,Greet,NULL);
    
    // Espera as threads terminarem de executar
    for(int i = 0; i < 5; i++) {
       if (pthread_join(t[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }
    return 0;
}