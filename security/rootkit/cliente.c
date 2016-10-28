#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define SERVER_PORT 81
#define BUF_SIZE    4096

void error(char*);

int main(int argc,char **argv)
{
        int c,s,bytes;		
	char buf[BUF_SIZE]; // Buffer donde recibimos el archivo
	struct hostent *h;  	    // Informacion del servidor
	struct sockaddr_in channel; // IP Address
	/* Manejo de errores muy pero muy sencillo*/	
	if(argc != 3) error("Uso: client server_name file_name");
	h = gethostbyname(argv[1]);
	//Ya que hemos obtenido una estructuda hostent
	if(!h) 	      error("ERROR: Funcion gethostbyname fallo");
	s = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(s<0) error("ERROR: En SOCKET");
	memset(&channel,0,sizeof(channel));
	channel.sin_family = AF_INET;
	memcpy(&channel.sin_addr.s_addr,h->h_addr,h->h_length);
	channel.sin_port = htons(SERVER_PORT);
	/* Nos conectamos al servidor*/
	c= connect(s,(struct sockaddr*)&channel, sizeof(channel));
	if(c<0)		error("ERROR:Conexion fallo");
	/*Conexion establecida */
	write(s,argv[2],strlen(argv[2])+1);
	/* Obtenemos el archivo y lo mostramos en la salida estandar*/
	while(1){
		bytes =read(s,buf,BUF_SIZE);
		if(bytes <= 0){ write(1,"END\n",5); exit(0);}
		write(1,buf,bytes);
	}

	return 0;

}

void error(char *mensaje){
	printf("%s \n",mensaje);
	exit(1);
}
