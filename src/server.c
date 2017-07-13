#include "includes.h"
 
// Função da thread
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    int socket_desc, client_sock, c;
	char ip[16];
    struct sockaddr_in server, client;
     
    // Criação do socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Não foi possivel a criação do socket");
    }
    puts("Socket criado!!!");
     
    // Preparando o struct sockaddr_in
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8002 );
     
    // Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Falha no bind!!!");
        return 1;
    }
     
    // Listen com uma fila de tamanho 3
    listen(socket_desc , 3);
     
    c = sizeof(struct sockaddr_in);
      
	pthread_t thread_id;
	
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
						
		printf("Conexão aceita: ");
 		read(client_sock, ip, 11);        
		if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0){
			perror("Não foi possivel criar a thread!!!");
				return 1;
		}    

		printf("Beaglebone - %s\n", ip);

	}
				
		
     
    if (client_sock < 0)
    {
        perror("Falha ao conectar!!!");
        return 1;
    }
     
    return 0;
}
 
/*
 * Função da thread
i * */
void *connection_handler(void *socket_desc)
{
    //Pega o socket descrito
    int sock = *(int*)socket_desc;
    int read_size;
    int value, temperatura;
    char *message , client_message[2000];
	char ip[16];

 	read(sock, ip, 16);        
    //Envia uma mensagem ao cliente
    message = (char*)malloc(2000*sizeof(char*));
    strcpy (message, "Conectado ao servidor!\n");
    send(sock , message , 2000, 0);
	free(message);

    //Recebe o valor lido pelo sensor
    while( (read_size = read(sock, &value, 4)) > 0 ){

		//Calcula a temperatura ambiente
		printf("%f\n", ((value/22.75)*2));
    }
     
    // Cliente desconecta
    if(read_size == 0 || read_size == -1){
        printf("Beaglebone desconectada - %s\n", ip);
        fflush(stdout);
    }
         
    return 0;
} 
