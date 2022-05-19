#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int contador = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond_comeco, x_cond_final;

void *Greet (void *arg) {
    printf("Seja bem-vindo!\n");
    pthread_mutex_lock(&x_mutex);
    contador++;
    pthread_cond_broadcast(&x_cond_comeco);
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

void *ST (void *arg) {
    pthread_mutex_lock(&x_mutex);
    if(contador < 1){
        pthread_cond_wait(&x_cond_comeco,&x_mutex);
    }
    printf("Fique a vontade.\n");
    contador++;
    if(contador > 3){
        pthread_cond_signal(&x_cond_final);
    }
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

void *ST1 (void *arg) {
    pthread_mutex_lock(&x_mutex);
    if(contador < 1){
        pthread_cond_wait(&x_cond_comeco,&x_mutex);
    }
    printf("Sente-se por favor.\n");
    contador++;
    if(contador > 3){
        pthread_cond_signal(&x_cond_final);
    }
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

void *ST2 (void *arg) {
    pthread_mutex_lock(&x_mutex);
    if(contador < 1){
        pthread_cond_wait(&x_cond_comeco,&x_mutex);
    }
    printf("Aceita um copo d'agua?\n");
    contador++;
    if(contador > 3){
        pthread_cond_signal(&x_cond_final);
    }
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

void *GB (void *arg) {
    pthread_mutex_lock(&x_mutex);
    if(contador < 4){
        pthread_cond_wait(&x_cond_final,&x_mutex);
    }
    printf("Volte sempre!\n");
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]){
    pthread_t t[5];
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond_comeco, NULL);
    pthread_cond_init(&x_cond_final, NULL);

    pthread_create(&t[0],NULL,GB,NULL);
    pthread_create(&t[1],NULL,ST,NULL);
    pthread_create(&t[2],NULL,ST1,NULL);
    pthread_create(&t[3],NULL,ST2,NULL);
    pthread_create(&t[4],NULL,Greet,NULL);

    for(int i = 0; i < 5; i++) {
       if (pthread_join(t[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }

    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond_comeco);
    pthread_cond_destroy(&x_cond_final);
}