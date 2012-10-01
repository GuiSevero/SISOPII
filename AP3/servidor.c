#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "error.h"

#include <stdio.h>
#include <sys/socket.h>		
#include <arpa/inet.h>         
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>   

#define PORTNUMBER 2345


//Define o número máximo de threads
#include <pthread.h>
#define MAX_THREADS 5

int tamr;
char buffer[100];
int serversocket;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;
int socketsize;
pthread_t thread[MAX_THREADS];

     
void *criaThread(void *con) {
	int connectionsocket = con;	

	//Lendo mensagem enviada pelo cliente
         tamr = read(connectionsocket, buffer, 100);
				 buffer[tamr] = '\0';	
         printf("Cliente %s enviou mensagem %s", inet_ntoa(client_addr.sin_addr), buffer);
         
         if(strstr(buffer, "desligar")){
            //Encerra o socket servidor 
            close(serversocket);  
            //Finaliza o servidor
            exit(0);
         }
         //Encerra a conexao com o cliente
         close(connectionsocket);
}


   

int main(){
   int connectionsocket;
   int flag;

   	//Define um vetor de threads
	pthread_t threads[MAX_THREADS];
	int i;
    
   //Criacao do socket no servidor
   printf("Iniciando o servidor. \n");
   if ((serversocket = socket(AF_INET,SOCK_STREAM,0))<0){
      perror("Erro ao criar socket do servidor: \n");
   }   

   //Configuracao do endereco do servidor que sera ligado ao socket
   //zera toda a estrutura
   memset(&server_addr, 0, sizeof(server_addr));

   //Tipo de endereco
   server_addr.sin_family = AF_INET;
   //especificacao do endereco
   //server_addr.sin_addr.s_addr   = INADDR_ANY;

   //Querendo especificar manualmente um endereco pode-se usar
   server_addr.sin_addr.s_addr   = inet_addr("127.0.0.1");

   //Numero da porta
   server_addr.sin_port   = htons(PORTNUMBER);
   //
   //memset(&(server_addr.sin_zero), '\0', sizeof(server_addr.sin_zero));

   //Ligando o endereco do servidor ao socket
   if (bind(serversocket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))<0){
      perror("Erro de ligacao de socket no servidor: \n");
   } 

   //Indica que o socket deve "escutar"
   if (listen(serversocket, 1)<0){
      perror("Erro ao inicializar a escuta do socket no servidor: \n");
   }

   printf("Servidor escutando na porta: %d .\n", PORTNUMBER);

   //Servidor fica em loop para receber conexoes
   int countThread = 1;
   while(1){

      pthread_t child;
	
      socketsize = sizeof(client_addr);
      
      //Servidor aguardando por conexao
      connectionsocket = accept(serversocket, (struct sockaddr *)&client_addr, &socketsize);
	
      if(connectionsocket < 0){ 
         perror("Erro ao receber pedido de conexao: \n");
      }else{
         printf("Conexao estabelecida com cliente %s. \n", inet_ntoa(client_addr.sin_addr));
       
	pthread_create(&thread[i], NULL, (void *)criaThread, (int *)connectionsocket); 
	printf("Criei a thread = %d\n",countThread);
        countThread++;

      }
	
	
   }
}
