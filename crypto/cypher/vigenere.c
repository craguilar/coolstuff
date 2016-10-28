/*
Programa hecho por: Carlos Ruiz Aguilar. 
Descripcion : Se trata de una version sencilla del algoritmo de Vigenere.
Compilacion : gcc vigenere.c -o vigenere
Run         : ./vigenere

Algunas consdieraciones , como alfabeto se toma el código ASCII de 127
caracteres,por lo que los valores se calculan en base a su valor ASCII.

IMPORTANTE: 

EL LIMITE DEL mensaje en claro , mensaje oculto y k ES DE
256 CARACTERES Y SE ENCUNTRA DEFINIDO MEDIANTE MAX_BUFF.

*/

#include<stdio.h>
#include<string.h>
#include<stdio.h>

#define MAX_BUFF 256

/*Definicion de funciones*/
int   mod(int, int);
char* cifrado(char*,char*);
char* descifrado(char*,char*);
int    menu(void);
/* Main*/
void main(void){
	do{}while(menu()!=0);

}
/* Formula para obtener el modulo de cualquier numero , ya que
   el operador % unicamente es viable para numeros positivos , esta
   formula permite extender al calculo de valores negativos*/
int mod(int a, int b)
{ 
	return (a%b+b)%b; 
}
/* Funcion de cifrado, es sencilla implica un desplazamiento o suma 
   dentro del alfabeto a usar*/
char* cifrado(char* mensaje , char* k){
	int n=strlen(mensaje);
	int j=0;
	int i=0;
	int suma;
	printf("c(texto oculto)>");
	for(i=0;i<n;i++){
		//Podriamos decir que este es el elemento central de
		//nuestro algoritmo de cifrado
		suma=mod(mensaje[i]+k[j],127);
		printf("%c",suma);
		j++;
		if(j==strlen(k))
		   j=0;
	}
	printf("<FIN \n\n####################################");
	return NULL;
}
/* Funcion de descifrado se trata de un proceso inverso  al cifrado, que 
   por obviedad se trata de una resta*/
char* descifrado(char* c, char* k){
	int n=strlen(c);
        int j=0;
	int i;
        int suma;
        printf("m(mensaje)>");
        for(i=0;i<n;i++){
                suma=mod((c[i]-k[j]),127);
                printf("%c",suma);
                j++;
                if(j==strlen(k))
                   j=0;
        }
        printf("\n\n####################################");

	return NULL;
}
/* Funcion que nos permite estructurar la logica de un menu para el programa, asi
   como pedir algunos datos*/
int menu(){
	char op[MAX_BUFF]="c";
	char correcto=0;
	char buff[MAX_BUFF];
	char k[MAX_BUFF];
	printf("####################################\n");
	printf("Cifrado Vigenere- UNAM, Criptografia\n");
	printf("####################################\n");
	printf("Que operacion desea realizar?\n\tc)Cifrado\n\td)Descifrado\n\ts)Salir");
	do{
		printf("\n\n[c/d/s]>");
		gets(op);
		if(strcmp(op,"c")==0){
			correcto=1;
			printf("m(mensaje)>");
			gets(buff);		
			printf("k(llave)>");
			gets(k);
			cifrado(buff,k);
				
		}
		else if(strcmp(op,"d")==0){
			correcto=1;
			printf("c(texto oculto)>");
			gets(buff);
			printf("k(llave)>");
			gets(k);
			descifrado(buff,k);
		}
		else if(strcmp(op,"s")==0)
			return 0;
				
	}
	while(correcto!=1);
	return 1;
			
}

