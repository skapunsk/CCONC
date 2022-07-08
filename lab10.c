#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>


#define L 10 //numero de threads leitoras
#define E 4 //numero de threads escritoras

int l = 0, e = 0; //contadores de threads lendo e escrevendo (globais)

sem_t em_e, em_l; // semaforos para exclusao mutua 
sem_t escr, leit; // semaforos para condição


//funcao executada pelos leitores
void * leitor (void * arg) {
    int *id = (int *) arg;
    while(1) {
        sem_wait(&leit);
        sem_wait(&em_l); 
        printf("Leitora %d esta querendo ler\n", *id);
        l++; if(l==1) sem_wait(&escr);
        sem_post(&em_l);                      
        sem_post(&leit);
        printf("Leitora %d esta lendo\n", *id);
        //le...
        sem_wait(&em_l); l--; 
        printf("Leitora %d acabou de ler\n", *id);
        if(l==0) sem_post(&escr);
        sem_post(&em_l);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

//funcao executada pelos escritores
void * escritor (void * arg) {
    int *id = (int *) arg;
    while(1) {
        sem_wait(&em_e); 
        e++; if(e==1) sem_wait(&leit);
        printf("Escritora %d quer escrever\n", *id);
        sem_post(&em_e);
        sem_wait(&escr);     
        printf("Escritora %d esta escrevendo\n", *id);                  
        //escreve...
        printf("Escritora %d acabou de escrever\n", *id);
        sem_post(&escr);
        sem_wait(&em_e); e--; 
        if(e==0) sem_post(&leit);
        sem_post(&em_e);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) { 
    pthread_t tid[L+E]; 
    int t, id[L+E];   

    // inicia os semaforos
    sem_init(&em_l, 0, 1);
    sem_init(&leit, 0, 1);
    sem_init(&em_e, 0, 1);      
    sem_init(&escr, 0, 1);      

    //cria as threads leitoras
    for(t=0; t<L; t++) {
        id[t] = t+1;
        if(pthread_create(&tid[t], NULL, leitor, (void *) &id[t])) exit(-1);
    } 

    //cria as threads escritoras
    for(t=0; t<E; t++) {
        id[t+L] = t+1;
        if(pthread_create(&tid[t+L], NULL, escritor, (void *) &id[t+L])) exit(-1);
    } 

    //--espera todas as threads terminarem
    for (t = 0; t < L+E; t++) {
        pthread_join(tid[t], NULL);
    }
}
