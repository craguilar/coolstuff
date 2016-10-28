/*
HECHO POR   : Carlos Ruiz Aguilar
DESCRIPCION : Programa que recibe por linea de comandos los parametros x y z, para obtener:
	     x^y mod z
	     Se hace uso de enteros grandes en la construccion de este programa.
Compilacion : gcc -o xymodz xymodz.c -lgmp
Run         : ./xymodz x y z
Dependencias: Debera tener instalado el paquete libgmp-dev en Linux
*/
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

int hexflag=0;
void xymodz(char*,char*,char*);

void main(int argc,char** argv){
	/*Validacion de parametros de linea de comandos */
	if(argc<4){	
		printf("ERROR: Run ./xymodz [-h] x y z\n");
		exit(EXIT_FAILURE);
	}
	else if(strcmp(argv[1],"-d")==0&&argc<5){
		printf("ERROR: Run ./xymodz [-h] x y z\n");
		exit(EXIT_FAILURE);
	}
	else if(strcmp(argv[1],"-h")==0&&argc==5){
		printf("LOG: Procesamiento hexadecimal\n");
		hexflag=1;
	}
	/* Procesamos la informacion */
	if(hexflag==1)
		xymodz(argv[2],argv[3],argv[4]);
	else
		xymodz(argv[1],argv[2],argv[3]);
	
}
void xymodz(char* x_c, char* y_c , char* z_c){
	int base=10;
	mpz_t r,x,y,z;
	char* resultado_10=NULL;
	char* resultado_16=NULL;
	mpz_init(x);
	mpz_init(y);
	mpz_init(z);
	mpz_init(r);
	if(hexflag==1)	base=16;
	mpz_set_str(x,x_c,base);
	mpz_set_str(y,y_c,base);
	mpz_set_str(z,z_c,base);
	/*FUNCION USADA*/
	mpz_powm(r,x,y,z);
 	printf("\nRESULTADOS\n\nDEC\n");
	mpz_out_str(stdout  ,10,r);
 	printf("\nHEX\n");
	mpz_out_str(stdout  ,16,r);
	printf("\n\n");
	/* CLEAR */	
	mpz_clear(x);
	mpz_clear(y);
	mpz_clear(z);
}
