//http://www.slideshare.net/dragonjar/hide-and-find-rootkits-linux
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define  SERVER_PORT 81
#define  BUF_SIZE    4096
#define  QUEUE_SIZE  10

void error(char*);

int  main(int argc,char **argv){
	
	int s, b, l , fd,sa, bytes, on=1;
	char buf[BUF_SIZE];
	/* Estructura para manejar direcciones de internet*/
	struct sockaddr_in canal;
	/* Reservamos memoria inicializada a cero*/
	memset(&canal,0,sizeof(canal));
	// Inicializacion de el canal
	canal.sin_family = AF_INET;
	// Indicamos que escuche por una especie de comodin
	// de direcciones de internet 
	canal.sin_addr.s_addr = htonl(INADDR_ANY);
	// Puerto
	canal.sin_port=htons(SERVER_PORT);
        // Creamos el socket al igual que en el servidor
	s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	// Unicamente por debug debe quitarse
	if(s<0) error("ERROR: Fallo SOCKET");
	/*Modificamos las opciones del socket, donde s es
	  el file descriptor del socjet, SOL_SOCKET socket
	  level, SO_REUSEADDR especifica una opcion, con on 
	  indicamos que la opcion esta habilita*/
	setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char *)&on, sizeof(on));
        //ligamos una direccion de internet al socket
	b= bind(s,(struct sockaddr *)&canal,sizeof(canal));
	if(b<0) error("ERROR: Fallo BIND");
	//Escuchamos 
	l=listen(s,QUEUE_SIZE);
	if(l<0) error("ERROR: Fallo LISTEN");
	
	while(1){
		// Se acepta una conexion y no queremos registrar
		// la informacion de la conexion entrante 0,0
		sa= accept(s,0,0);
		if(sa<0) error("ERROR: Fallo ACCEPT");
		//Leemos la peticion
		read(sa,buf,BUF_SIZE);
		//printf("%s",buf);
		// Abrimos el archivo en modo solo lectura
		fd= open(buf,O_RDONLY);
		if(fd<0) error("ERROR: Fallo open file");
		while(1){
			bytes =read(fd,buf,BUF_SIZE);
			if(bytes <=0) break;
			write(sa,buf,bytes);
		}
		//Cerramos conexion y archivo
		close(fd);
		close(sa);
	}
	return 0;

}

void error(char* mensaje){
	printf("%s \n",mensaje);
	exit(1);
}	
