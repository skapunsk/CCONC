
// Monitor que implementa a logica do padrao leitores/escritores
class LE {
  private int leit, escr;  
  
  // Construtor
  LE() { 
     this.leit = 0; //leitores lendo (0 ou mais)
     this.escr = 0; //escritor escrevendo (0 ou 1)
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor (int id) {
    try { 
      while (this.escr > 0) {
      //if (this.escr > 0) {
         System.out.println ("le.leitorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.leit++;  //registra que ha mais um leitor lendo
      System.out.println ("le.leitorLendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id) {
     this.leit--; //registra que um leitor saiu
     if (this.leit == 0) 
           this.notify(); //libera escritor (caso exista escritor bloqueado)
     System.out.println ("le.leitorSaindo("+id+")");
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id) {
    try { 
      while ((this.leit > 0) || (this.escr > 0)) {
      //if ((this.leit > 0) || (this.escr > 0)) {
         System.out.println ("le.escritorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.escr++; //registra que ha um escritor escrevendo
      System.out.println ("le.escritorEscrevendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id) {
     this.escr--; //registra que o escritor saiu
     notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
     System.out.println ("le.escritorSaindo("+id+")");
  }
}

//Classe da variavel global com um set e get para poder acessar o valor ou mudalo
class Global {
  private int var;

  // Construtor
  Global (int inicial) {
    this.var = inicial;
  }
  public int getVar() {
    return var;
  }

  public void setVar(int var) {
    this.var = var;
  }

}

// Escritor
class T1 extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo...
  LE monitor; //objeto monitor para coordenar a lógica de execução das threads
  Global varglobal; // variavel global compartilhada entre threads

  // Construtor
  T1 (int id, int delayTime, LE m, Global v) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
    this.varglobal = v;
  }

  // Método executado pela thread
  public void run () {
    try {
      for (;;) {
        this.monitor.EntraEscritor(this.id);
        varglobal.setVar(varglobal.getVar()+1); // (var = var + 1) usando os metodos de Global
        this.monitor.SaiEscritor(this.id);
        sleep(this.delay); //atraso bobo...
      }
    } catch (InterruptedException e) { return; }
  }
}

// Leitor
class T2 extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads
  Global varGlobal; // variavel global compartilhada entre threads

  // Construtor
  T2 (int id, int delayTime, LE m, Global v) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
    this.varGlobal = v;
  }

  // Método executado pela thread
  public void run () {
    try {
      for (;;) {
        this.monitor.EntraLeitor(this.id);
        if(this.varGlobal.getVar() % 2 == 0) {System.out.println ("Variavel esta par");}
        else {System.out.println("Variavel esta impar");}
        this.monitor.SaiLeitor(this.id);
        sleep(this.delay); 
      }
    } catch (InterruptedException e) { return; }
  }
}

//LeitorEscritor
class T3 extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads
  Global varGlobal; // variavel global compartilhada entre threads

  // Construtor
  T3 (int id, int delayTime, LE m, Global v) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
    this.varGlobal = v;
  }

  // Método executado pela thread
  public void run () {
    double j = 777777777.7;
    int i;
    try {
      for (;;) {
        this.monitor.EntraLeitor(this.id);
        System.out.println ("A variavel esta com o valor : "+this.varGlobal.getVar());
        this.monitor.SaiLeitor(this.id);
        sleep(this.delay);
        for (i=0; i<100000000; i++) {j=j/2;} // processamento significativo
        this.monitor.EntraEscritor(this.id);
        this.varGlobal.setVar(this.id);
        this.monitor.SaiEscritor(this.id);
        sleep(this.delay);
      }
    } catch (InterruptedException e) { return; }
  }

}

// Classe principal
public class Lab8 {
  static final int L = 4;
  static final int E = 3;
  static final int LE = 3;

  public static void main (String[] args) {
    int i;
    Global varGlobal = new Global(0);
    LE monitor = new LE();  // Monitor (objeto compartilhado entre leitores e escritores)
    T1[] tEsc = new T1[E];  // Threads leitores
    T2[] tLei = new T2[L];  // Threads escritores
    T3[] tLeiEsc = new T3[LE]; // Threads leitores/escritores

    //inicia o log de saida
    for (i=0; i<E; i++) {
      tEsc[i] = new T1(i+1, (i+1)*500, monitor, varGlobal);
      tEsc[i].start();
    }
    for (i=0; i<L; i++) {
      tLei[i] = new T2(i+1, (i+1)*500, monitor, varGlobal);
      tLei[i].start();
    }
    for (i=0; i<LE; i++) {
      tLeiEsc[i] = new T3(i+1, (i+1)*500, monitor, varGlobal);
      tLeiEsc[i].start();
    }

  }
}
