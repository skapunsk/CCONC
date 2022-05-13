#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"

int nthreads, *vetorEntrada, i_global = 0; // número de threads que executaram, vetor randomizado e controlador das threads
float *vetorSaida, *vetorSaidaT; // Vetor da Saida Sequencial e Vetor da Saida Concorrente
long long int dim; // dimensão do vetor a ser randomizado
pthread_mutex_t mutex;

//Inicializa o vetor com números aleatorios 
void initVetor(int dim){
    for(int i=0; i<dim; i++){
        int n = rand() / 100000;
        vetorEntrada[i] = n;    
    }

}

// retorna 1 se o número é primo retorna 0 se o número é composto
int ehPrimo (long long int n){
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2 ==0) return 0;
    for (int i=3; i<sqrt(n)+1; i+=2)
        if(n%i ==0) return 0;
    return 1; 
}

void processaPrimos(int vetorEntrada[], float vetorSaida[], int dim) {
    for(int i=0; i<dim; i++) {
        if (ehPrimo(vetorEntrada[i])) //É primo
            vetorSaida[i] = sqrt(vetorEntrada[i]);
        else
            vetorSaida[i] = vetorEntrada[i];
    }
}

/* Tarefa que as threads executaram para achar os primos e alocar no vetor de saída:
    Primeiro ela aloca o valor do controlador global para uma variavel local i e seta o controlador global
    para indicar o próximo número no vetor a ser checado. Então ele checa se esse o numero em vetor[i]
    é primo, se sim ela adiciona a raiz quadrada dele ao vetor de saida e se não ele adiciona ele mesmo ao
    vetor de saida. Depois é novamente atribuido a i o valor do controlador global e são repetidos a checagem
    até i >= dim
*/
void *tarefa (void * arg){
    int i;
    pthread_mutex_lock(&mutex);
    i = i_global;
    i_global++;
    pthread_mutex_unlock(&mutex);
    while(i < dim){
        if(ehPrimo(vetorEntrada[i]))
            vetorSaidaT[i] = sqrt(vetorEntrada[i]);
        else
            vetorSaidaT[i] = vetorEntrada[i];
        pthread_mutex_lock(&mutex);
        i = i_global;
        i_global++;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void comparaVet(float vetor[], float vetor2[]){
    for(int i=0; i<dim; i++){
        if(vetorSaida[i] != vetorSaidaT[i]){
            printf("valores diferente na posição [%d]\n", i);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    pthread_t *tid;
    double comeco, final, delta;
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);

    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1; 
    }
    // Alocando as entradas recebidas no programa as variaveis globais
    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    //Alocando espaço de memoria para os vetores
    vetorEntrada = (int*) malloc(sizeof(int)*dim);
    if(vetorEntrada == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 2;
    }
    vetorSaida = (float*) malloc(sizeof(float)*dim);
    if(vetorSaida == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 2;
    }
    vetorSaidaT = (float*) malloc(sizeof(float)*dim);
    if(vetorSaidaT == NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 2;
    }

    //inicializando o vetor de números
    initVetor(dim);

    //Execução sequencial
    GET_TIME(comeco);
    processaPrimos(vetorEntrada,vetorSaida,dim);
    GET_TIME(final);
    delta = final - comeco;
    printf("Tempo sequencial: %lf\n", delta);

    GET_TIME(comeco);
    //Aloca o espaço para as threads
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {
       fprintf(stderr, "ERRO--malloc\n");
       return 2;
    }

    //Cria as threads
    for(int i=0; i<nthreads; i++){
        if(pthread_create(&tid[i], NULL, tarefa, NULL)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;        
        }
    }    

    //espera as threads terminarem 
    for(int i=0; i<nthreads; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }
    GET_TIME(final);
    delta = final - comeco;
    printf("Tempo Concorrente: %lf\n", delta);
    pthread_mutex_destroy(&mutex);
    comparaVet(vetorSaida, vetorSaidaT);

    //libera a memoria alocada
    free(vetorEntrada);
    free(vetorSaidaT);
    free(vetorSaida);
    free(tid); 
}