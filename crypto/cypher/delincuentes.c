/*
Programa hecho por: Carlos Ruiz Aguilar. 
Descripcion : Se trata de una version sencilla del algoritmo de usado
	      por los delicuentes de la nota: 
	      http://m.insightcrime.org/pages/article/5337
Compilacion : gcc delincuentes -o delincuentes
Run         : ./delincuentes


IMPORTANTE: 

EL LIMITE DEL mensaje en claro , mensaje oculto y k ES DE 256 CARACTERES 
Y SE ENCUNTRA DEFINIDO MEDIANTE MAX_BUFF.

CONSIDERACIONES:
-En GZV a partir de la lectura de dos ZZ todo lo consecuente sera tomado 
 como numero.
-EN EL CIFRADO GZV, no se podran tener mensajes con dos AA consecutivas ya
 que lo consecuente se tomara coom un número.
*/

#include<stdio.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define MAX_BUFF 256

/*Limpiar cadena*/
int limpiarCadena(char*);
/*Alfabetos*/
int  esnumero(char);
char chartoGZM(char);
char chartoGZV(char);
char GZVtonum(char);
char GZMtochar(char);
char GZVtochar(char);
/*Definicion de funciones*/
char* cifradoGZM(char*);
char* descifradoGZM(char*);
char* cifradoGZV(char*);
char* descifradoGZV(char*);
int    menu(void);
/* Main*/
void main(void){
	do{}while(menu()!=0);

}
int limpiarCadena(char* m){
	char* buffer=(char *)malloc(sizeof(char));
	int   n= strlen(m);
	int   i,j=0;
        for(i=0;i<n;i++){ 
        	m[i]=toupper(m[i]);
                if((m[i]!=45)&&(m[i]!=33)&&(m[i]<48||m[i]>57)&&(m[i]<63 || m[i]>90)){
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
int   esnumero(char c){
	if( c=='0' || c=='1' || c=='2' ||
	    c=='3' || c=='4' || c=='5' ||
	    c=='6' || c=='7' || c=='8' ||
            c=='9')
		return 1;
	else
		return 0;
}
char  GZVtonum(char c){
	int r;
	switch(c){
		
		case 'W':
			r='0';
			break;
		case 'Z':
			r='1';
			break;
		case 'M':
			r='2';
			break;
		case 'L':
			r='3';
			break;
		case 'E':
			r='4';
			break;
		case 'X':
			r='5';
			break;
		case 'J':
			r='6';
			break;
		case 'R':
			r='7';
			break;
		case 'I':
			r='8';
			break;
		case 'B':
			r='9';
			break;
	}
	return r;
}
char  GZMtochar(char c){
	int r;	
	switch(c){
		case 'Y':
			r='A';
			break;
		case 'T':
			r='B';
			break;
		case 'Z':
			r='C';
			break;
		case 'V':
			r='D';
			break;
		case 'U':
			r='E';
			break;
		case 'H':
			r='F';
			break;
		case 'C':
			r='G';
			break;
		case 'N':
			r='H';
			break;
		case -37:
			r='I';
			break;
		case 'B':
			r='J';
			break;
		case 'F':
			r='K';
			break;
		case 'R':
			r='L';
			break;
		case 'P':
			r='M';
			break;
		case 'S':
			r='N';
			break;
		case 'A':
			r=-37;
			break;
		case 'E':
			r='O';
			break;
		case 'I':
			r='P';
			break;
		case 'L':
			r='Q';
			break;
		case 'X':
			r='R';
			break;
		case 'G':
			r='S';
			break;
		case 'Q':
			r='T';
			break;
		case 'M':
			r='U';
			break;
		case 'O':
			r='V';
			break;
		case 'D':
			r='W';
			break;
		case 'K':
			r='X';
			break;
		case 'W':
			r='Y';
			break;
		case 'J':
			r='Z';
			break;
		case '!':
			r='@';
			break;
		case '-':
			r='?';
			break;
	}
	return r;
}
char  GZVtochar(char c){
	int r;
	switch(c){
		case 'Z':
			r='A';
			break;
		case 'I':
			r='B';
			break;
		case 'O':
			r='C';
			break;
		case 'P':
			r='D';
			break;
		case 'K':
			r='E';
			break;
		case 'C':
			r='F';
			break;
		case 'V':
			r='G';
			break;
		case 'B':
			r='H';
			break;
		case 'U':
			r='I';
			break;
		case 'X':
			r='J';
			break;
		case 'R':
			r='K';
			break;
		case 'A':
			r='L';
			break;
		case 'Y':
			r='M';
			break;
		case 'T':
			r='N';
			break;
		case 'L':
			r=-37;
			break;
		case 'D':
			r='O';
			break;
		case 'H':
			r='P';
			break;
		case -37:
			r='Q';
			break;
		case 'E':
			r='R';
			break;
		case 'N':
			r='S';
			break;
		case 'M':
			r='T';
			break;
		case 'J':
			r='U';
			break;
		case 'S':
			r='V';
			break;
		case 'Q':
			r='W';
			break;
		case 'G':
			r='X';
			break;
		case 'F':
			r='Y';
			break;
		case 'W':
			r='Z';
			break;
		case '!':
			r='@';
			break;
		default:
			r='?';
	}
	return r;
}
char  chartoGZM(char c){
	int r;
	switch(c){
		case 'A':
			r='Y';
			break;
		case 'B':
			r='T';
			break;
		case 'C':
			r='Z';
			break;
		case 'D':
			r='V';
			break;
		case 'E':
			r='U';
			break;
		case 'F':
			r='H';
			break;
		case 'G':
			r='C';
			break;
		case 'H':
			r='N';
			break;
		case 'I':
			r=164;
			break;
		case 'J':
			r='B';
			break;
		case 'K':
			r='F';
			break;
		case 'L':
			r='R';
			break;
		case 'M':
			r='P';
			break;
		case 'N':
			r='S';
			break;
		case -37:
			r='A';
			break;
		case 'O':
			r='E';
			break;
		case 'P':
			r='I';
			break;
		case 'Q':
			r='L';
			break;
		case 'R':
			r='X';
			break;
		case 'S':
			r='G';
			break;
		case 'T':
			r='Q';
			break;
		case 'U':
			r='M';
			break;
		case 'V':
			r='O';
			break;
		case 'W':
			r='D';
			break;
		case 'X':
			r='K';
			break;
		case 'Y':
			r='W';
			break;
		case 'Z':
			r='J';
			break;
		case '@':
			r='!';
			break;
		default:
			r='-';
	}
	return r;
}
char  chartoGZV(char c){
	int r;
	switch(c){
		case 'A':
			r='Z';
			break;
		case 'B':
			r='I';
			break;
		case 'C':
			r='O';
			break;
		case 'D':
			r='P';
			break;
		case 'E':
			r='K';
			break;
		case 'F':
			r='C';
			break;
		case 'G':
			r='V';
			break;
		case 'H':
			r='B';
			break;
		case 'I':
			r='U';
			break;
		case 'J':
			r='X';
			break;
		case 'K':
			r='R';
			break;
		case 'L':
			r='A';
			break;
		case 'M':
			r='Y';
			break;
		case 'N':
			r='T';
			break;
		case -37:
			r='L';
			break;
		case 'O':
			r='D';
			break;
		case 'P':
			r='H';
			break;
		case 'Q':
			r=164;
			break;
		case 'R':
			r='E';
			break;
		case 'S':
			r='N';
			break;
		case 'T':
			r='M';
			break;
		case 'U':
			r='J';
			break;
		case 'V':
			r='S';
			break;
		case 'W':
			r='Q';
			break;
		case 'X':
			r='G';
			break;
		case 'Y':
			r='F';
			break;
		case 'Z':
			r='W';
			break;
		case '@':
			r='!';
			break;
		case '0':
			r='W';
			break;
		case '1':
			r='Z';
			break;
		case '2':
			r='M';
			break;
		case '3':
			r='L';
			break;
		case '4':
			r='E';
			break;
		case '5':
			r='X';
			break;
		case '6':
			r='J';
			break;
		case '7':
			r='R';
			break;
		case '8':
			r='I';
			break;
		case '9':
			r='B';
			break;
		default:
			r='-';
	}
	return r;
}
char* cifradoGZV(char* mensaje){
	int i,n=limpiarCadena(mensaje);
	int nflag=0;
	printf("Su mensaje>%s<\n",mensaje);
	printf("Su c      >");
	for(i=0;i<n;i++){
		if(esnumero(mensaje[i])==0){
			printf("%c",chartoGZV(mensaje[i]));
			nflag=0;
		}
		else{ 
			if(nflag==0){
				printf("ZZ%c",chartoGZV(mensaje[i]));
				nflag=1;
			}
			else
				printf("%c",chartoGZV(mensaje[i]));
		}
	
	}
	printf("\n\n");
	return NULL;
}

char* descifradoGZV(char* c){
	int i,n=limpiarCadena(c);
	int nflag=0;
	printf("Su texto oculto>%s<\n",c);
	printf("Su m           >");
	for(i=0;i<n;i++){
		if(nflag==0&&(c[i]=='Z'&&(i<(n-1))&&c[i+1]=='Z')){
			
			i=i+2;
			printf("%c",GZVtonum(c[i]));
			nflag=1;
		}
		else if(nflag==1)
			printf("%c",GZVtonum(c[i]));
		else
			printf("%c",GZVtochar(c[i]));
	}
	printf("\n\n");
	return NULL;
}

char* cifradoGZM(char* mensaje ){
	int i,n=limpiarCadena(mensaje);
	printf("Su mensaje>%s<\n",mensaje);
	printf("Su c      >");
	for(i=0;i<n;i++)
		printf("%c",chartoGZM(mensaje[i]));
	printf("\n\n");
	return NULL;
}

char* descifradoGZM(char* c){
	int i,n=limpiarCadena(c);
	printf("Su texto oculto >%s<\n",c);
	printf("Su m            >");
	for(i=0;i<n;i++)
		printf("%c",GZMtochar(c[i]));
	printf("\n\n");
	return NULL;
}
/* Funcion que nos permite estructurar la logica de un menu para el programa, asi
   como pedir algunos datos*/
int menu(){
	char op[MAX_BUFF]="c";
	char correcto=0;
	char buff[MAX_BUFF];
	printf("####################################\n");
	printf("Cifrado Vigenere- UNAM, Criptografia\n");
	printf("####################################\n");
	printf("Que operacion desea realizar?\n\tm)Cifrado GZM\n\tv)Cifrado GZV\n\tu)Descifrado GZM\n\td)Descifrado GZV\n\ts)Salir");
	do{
		printf("\n\n[m/v/u/d/s]>");
		gets(op);
		if(strcmp(op,"m")==0){
			correcto=1;
			printf("m(mensaje)>");
			gets(buff);		
			cifradoGZM(buff);
		}
		else if(strcmp(op,"v")==0){
			correcto=1;
			printf("m(mensaje)>");
			gets(buff);		
			cifradoGZV(buff);		
		}
		else if(strcmp(op,"u")==0){
			correcto=1;
			printf("c(texto oculto)>");
			gets(buff);		
			descifradoGZM(buff);
		}
		else if(strcmp(op,"d")==0){
			correcto=1;
			printf("c(texto oculto)>");
			gets(buff);
			descifradoGZV(buff);
		}
		else if(strcmp(op,"s")==0)
			return 0;
				
	}
	while(correcto!=1);
	return 1;
			
}

