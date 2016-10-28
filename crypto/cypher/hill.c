/*
Programa hecho por: Carlos Ruiz Aguilar. 
Descripcion : Se trata de una version sencilla del algoritmo de Hill.
Compilacion : gcc hill.c -o hill
Run         : ./hill
*/

#include<stdio.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>

#define CPAD     'x'
#define MAX_BUFF 512
 
/*Definicion de funciones*/
int   mcd(int,int);
int   invmod(int,int); 
int   mod(int, int);
int   limpiarCadena(char* );
int   getDeterminante(int[][2]);
char* cifrado(char*,int[][2]);
char* descifrado(char*,int[][2]);
int   menu(void);

/* Main*/
void main(void){
	do{}while(menu()!=0);

}
/* 
   Formula para obtener  el modulo  de  cualquier  numero ,  ya que
   el operador % unicamente es viable para numeros positivos , esta
   formula permite extender al calculo de valores negativos.
*/
int getDeterminante(int a[][2]){
	int det=0;
	det=a[0][0]*a[1][1]-a[0][1]*a[1][0];
	return det;
}
int mcd(int x, int y)
{
    if(y==0)
        return x;
    else
        return mcd(y, x%y);
}
int invmod(int x,int b){
	int i=1;
	while(mod(i*x,b)!=1&&i<b)
	{ 
		i++; 
	}
	printf("invmod=%d\n",mod(i,b));
	return mod(i,b);
}
int mod(int a, int b)
{ 
	return (a%b+b)%b; 
}
int limpiarCadena(char* m){
        char* buffer=(char *)malloc(sizeof(char));
        int   n= strlen(m);
        int   i,j=0;
        for(i=0;i<n;i++){
                m[i]=toupper(m[i]);
                if(m[i]<65 || m[i]>90){
                        //Caracter invalido
                }
                else{
                        buffer[j]=m[i];
                        j++;
                        buffer=(char *)realloc(buffer, (j+1)*sizeof(char));
                }

          }
        buffer[j]='\0';
        for(i=0;i<j;i++)
                m[i]=buffer[i];
        if(j<n)
                m[j]='\0';
        /*Liberar Memoria de Buffer*/
        buffer =NULL;
        free(buffer);
        /*Fin de Liberacion de Memoria de Buffer*/
        return j;

}
char* cifrado(char* mensaje , int k[][2]){
	int n=strlen(mensaje);
	int  x;
	int  c;
	int  i,j,h;
	int  A[32][32];
	if(getDeterminante(k)==0){
		printf("ERROR : La matriz k es invertible el programa terminara\n");
		exit(EXIT_FAILURE);
	}
		
	else if(mcd(abs(getDeterminante(k)),26)!=1){
		printf("ERROR: No se puede obtener una matriz de descifrado\n");
		exit(EXIT_FAILURE);	
	}
	//Limpieza de la cadena
	n=limpiarCadena(mensaje);
	//Acomodamos m
	h=0;
	for(i=0;i<n;i++){
		A[h][j]=mensaje[i];
		j++;
		if(j==2){
			j=0;
			h++;
		}
			
	}
	x=h+mod(n,2);
	printf("c(texto oculto)>");
	//Multiplicacion de matrices
	for (i=0; i<x; i++){
		for (j=0; j<2; j++){         
			c=0;   
			for (h=0; h<2; h++){
				c = c + ((A[i][h]-65) * k[j][h]);
			}
			c=mod(c,26)+65;
			printf("%c",c);	
		}
   	}
	printf("\n");
	
	return NULL;
}
char* descifrado(char* c, int k[][2]){
	int  n=strlen(c);
	int  x, y;
	int  ch;
	int  i,j,h;
	int  inverso;
	int  determinante;
	int  A[32][32];
	if(getDeterminante(k)==0){
		printf("ERROR : La matriz k es invertible el programa terminara\n");
		exit(EXIT_FAILURE);
	}	
	else if(mcd(abs(getDeterminante(k)),26)!=1){
		printf("ERROR: No se puede obtener una matriz de descifrado\n");
		exit(EXIT_FAILURE);	
	}
	//Limpieza de la cadena
	n=limpiarCadena(c);
	//Acomodamos m
	h=0;
	for(i=0;i<n;i++){
		A[h][j]=c[i];
		j++;
		if(j==2){
			j=0;
			h++;
		}
			
	}
	x=h+mod(n,2);
	//Obtenemos inversa de la matriz
	determinante=mod(getDeterminante(k),26);
	inverso=invmod(determinante,26);
	i=k[0][0];
	k[0][0]=k[1][1];
	k[1][1]=i;
	k[0][1]=-1*k[0][1];
	k[1][0]=-1*k[1][0];
	
	k[0][0]=inverso*k[0][0];
	k[0][1]=inverso*k[0][1];
	k[1][0]=inverso*k[1][0];
	k[1][1]=inverso*k[1][1];

	//Multiplicacion de matrices
	printf("m(mensaje)>");
	for (i=0; i<x; i++){
		for (j=0; j<2; j++){         
			ch=0;   
			for (h=0; h<2; h++){
				ch = ch + ((A[i][h]-65) * k[j][h]);
			}
			ch=mod(ch,26)+65;
			printf("%c",ch);				
         	}
   	}
	printf("\n");
	return NULL;
}
/* 
   Funcion que nos permite estructurar la logica de un menu para el 
   programa, asi como pedir algunos datos
*/

int menu(){
	char op[MAX_BUFF]="c";
	char correcto=0;
	char buff[MAX_BUFF];
	int  k[2][2];
	
	printf("##############################################################\n");
	printf("Cifrado Hill- UNAM, Criptografia\n");
	printf("##############################################################\n");
	printf("El cifrado del Hill esta compuesto de los siguientes elementos\n\n");
	printf("\tk = [ a11 a12 ] y  un mensaje m\n");
	printf("\t    [ a21 a22 ]\n\n");
	printf("Que operacion desea realizar?\n\tc)Cifrado\n\td)Descifrado\n\ts)Salir");
	do{
		
		printf("\n\n[c/d/s]>");
		gets(op);
		if(strcmp(op,"c")==0){
			correcto=1;
			printf("m(mensaje)>");
			gets(buff);		
	 		printf("\nIndique los elementos de la llave k\n");
			printf("k(llave)\n");
			printf("\tElemento a11>");
			scanf("%d",&k[0][0]);			
			printf("\tElemento a12>");
			scanf("%d",&k[0][1]);			
			printf("\tElemento a21>");
			scanf("%d",&k[1][0]);			
			printf("\tElemento a22>");
			scanf("%d",&k[1][1]);			
			cifrado(buff,k);
		}
		else if(strcmp(op,"d")==0){
			correcto=1;
			printf("c(texto oculto)>");
			gets(buff);
	 		printf("\nIndique los elementos de la llave k\n");
			printf("k(llave)\n");
			printf("\tElemento a11>");
			scanf("%d",&k[0][0]);			
			printf("\tElemento a12>");
			scanf("%d",&k[0][1]);			
			printf("\tElemento a21>");
			scanf("%d",&k[1][0]);			
			printf("\tElemento a22>");
			scanf("%d",&k[1][1]);			
			descifrado(buff,k);
		}
		else if(strcmp(op,"s")==0)
			return 0;
				
	}
	while(correcto!=1);
	return 1;
}

