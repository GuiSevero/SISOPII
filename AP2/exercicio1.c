#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//Define o número máximo de threads
#define MAX_THREADS 5

void *imprimeMensagem(void *id)
{
   int tid;
   tid = (int)id;
   printf("Sou a thread %d!\n", tid);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{	
   	//Define um vetor de threads
   	pthread_t threads[MAX_THREADS];
   	int i;
	
	//Cria uma thread passando o indice i como indice de criação, para posterior impressão na função imprimeMensagem
	for(i=0;i<MAX_THREADS;i++){
    	pthread_create(&threads[i], NULL, imprimeMensagem, (void *)i);
    }

   	pthread_exit(NULL);
}