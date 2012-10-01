/** 
   O programa ap2_ex2.c efetua uma sequência de débitos e créditos.
   Considerando os possíveis problemas causados pela concorrência apresentados em aula responda:
   
   1) O programa apresenta algum problema relacionado a concorrência? Qual?
         O programa apresenta seção critíca nas threads debitar e creditar. Ambas não podem "perder processador"
         enquanto estão em execução. Como não podemos garantir que elas percam processador, devemos usar um semáfaro
         para garantir exclusão mútua entre as duas threads.

   2) Caso tenha algum problema, apresente uma nova versão do programa, com o problema solucionado.
      No código abaixo temos a solução utilizando semáforo. 

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMOPER 10
#define NUMTHREADS 3

//inicia o saldo
int saldo = 100;

//semaforo para garantir exclusao mutua no acesso a variavel saldo
sem_t sema_saldo;
 
void creditar(int valor);
void debitar(int valor);
void *threadcreditar(void *);
void *threaddebitar(void *);

int main(){
   
   //declaração das threads que fazem débitos 
   pthread_t threadsd[NUMTHREADS];
   //declaração das threads que fazem créditos
   pthread_t threadsc[NUMTHREADS];
   int i;
   
   //Inicialização do semáforo que irá controlar para não haver sobreposição de chamadas de débito/crédito
   sem_init(&sema_saldo, 0, 1);

   //inicialização das threads
   for (i=0; i<NUMTHREADS; i++){
      pthread_create(&threadsc[i], NULL, threadcreditar, NULL);
      pthread_create(&threadsd[i], NULL, threaddebitar, NULL);
   }

   //join das threads (sincronização com a thread principal)
   for (i=0; i<NUMTHREADS; i++){
      pthread_join(threadsc[i],NULL);
      pthread_join(threadsd[i],NULL);
   }
   
   printf("Saldo final na conta = %d \n", saldo);
}

//procedimento que efetua créditos na conta(saldo)
void creditar(int valor){

   sleep(1);
   sem_wait(&sema_saldo);
      printf("Creditando valor. \n");
      int saldo_conta = saldo + valor;
      printf("O novo valor sera %d. \n", saldo_conta);
      saldo = saldo_conta;
   sem_post(&sema_saldo);

}

//procedimento que efetua débitos na conta(saldo)
void debitar(int valor){
   sleep(1);
   sem_wait(&sema_saldo);
      printf("Debitando valor. \n");
      int saldo_conta = saldo - valor; 
      printf("O novo valor sera %d. \n", saldo_conta);
      saldo = saldo_conta;
   sem_post(&sema_saldo);
}

//função que será executada pelas threads tipo threadsc
void * threadcreditar(void *dummy){
   int i;
   for (i=0;i<NUMOPER;i++){
      creditar(100);
   }
}

//função que será executada pelas threads tipo threadsd
void * threaddebitar(void *dummy){
   int i;
   for (i=0;i<NUMOPER;i++){
      debitar(100);
   }
}