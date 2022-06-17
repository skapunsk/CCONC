import java.util.Random;

//Classe da estrutura de dados (recurso) compartilhado entre as threads.
class VetorInfos {
    private int vetor[];    //vetor de inteiros randomicos.
    public int dimVetor;    //dimensao do vetor de numeros inteiros.
    private int nParesC=0;  //quantidade de numeros pares no vetor checados concorrentemente.

    //construtor
    public VetorInfos(int vetor[]) {
        this.vetor = vetor;
        this.dimVetor = vetor.length;
    }

    // checa se o numero no vetor eh par, se ele for, aumenta o contador.
    public synchronized void checaPar(int pos){
        if(vetor[pos]%2==0){
            this.nParesC++;
        }
    }

    //retorna a quantidade de numeros pares no vetor[]
    public synchronized int getnParesC(){
        return this.nParesC;
    }
}

//Classe que estende Thread e implementa a tarefa de cada thread do programa.
class T extends Thread{
    private int id; //identificador da thread
    private int NTHREADS;   //número de threads
    private int bloco;  //tamanho do bloco
    private VetorInfos vetorInfos;  //objeto compartilhado entre as threads

    //construtor
    public T(int id, int NTHREADS, int bloco, VetorInfos vetorInfos){
        this.id = id;
        this.NTHREADS = NTHREADS;
        this.bloco = bloco;
        this.vetorInfos = vetorInfos;
    }

    //método main das threads
    public void run(){
        final int start = id*this.bloco; //posicao inicial do bloco
        final int end;  //posição final do bloco

        if(this.id == NTHREADS-1){
            end = this.vetorInfos.dimVetor;  //trata o resto
        }else{
            end = (id+1)*(this.bloco);
        }

        //checa a quantidade de numeros pares no bloco
        for(int i = start; i<end; i++){
            this.vetorInfos.checaPar(i);
        }
    }
}

//Classe da aplicação.
class NumPares {
    static final int dimVetor = 10000;  //dimensao do vetor
    static final int NTHREADS = 2;  //numero de threads
    static final int bloco = dimVetor/NTHREADS;  //tamanho do bloco
    private static int nParesS = 0;

    public static void main (String[] args) {

        Thread[] threads = new Thread[NTHREADS];

        //Cria um vetor com numeros randomicos do tamanho de dimVetor
        Random rand = new Random();
        int[] vetor = new int[dimVetor];
        for(int i=0; i<dimVetor; i++){
            vetor[i] = rand.nextInt(dimVetor);
        }

        //cria uma instancia do recurso compartilhado entre as threads
        VetorInfos vetorInfos = new VetorInfos(vetor);

        //cria as threads da aplicacao
        for (int i = 0; i < threads.length; i++){
            threads[i] = new T(i, NTHREADS, bloco, vetorInfos);
        }

        //inicia as threads
        for (int i=0; i<threads.length; i++){
            threads[i].start();
        }

        //espera pelo termino de todas as threads
        for (int i = 0; i<threads.length; i++){
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        //implementação sequencial da checagem de quantidade de pares
        for(int i = 0; i < dimVetor; i++){
            if(vetor[i]%2==0)
                nParesS++;
        }

        //comparação entre a execução sequencial e a execução concorrente
        if (nParesS == vetorInfos.getnParesC()){
            System.out.println("O vetor tem " + vetorInfos.getnParesC() + " numeros pares.");
        } else {
            System.out.println("O número de pares não condiz\n");
        }
    }
}