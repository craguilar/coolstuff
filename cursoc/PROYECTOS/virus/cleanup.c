/* 
   Autor: Carlos Ruiz Aguilar
   Fecha: Enero , 2014
   Algunas consideraciones:
	1.Conocemos el MD5 del virus
	2.Asumimos que si existe un archivo .X21 y su pareja
   Requerimientos del programa:
	1.Rutina de desinfeccion cruiz.c
	2.Volver a todos a los archivos a su estado original (eliminar los virus).
	3.Debe hacerse un programa funcional (al menos 3).
	4. Fijar sobre terminacion variable del archivo de infeccion(-e)
	5. Recursividad
	6. Si recibe (-r) es recursivo
   NOTAS:
   La recursividad no se logra del todo habria que revisar mas acerca del tema
   Una implementacion mas atinada de una pila: http://www.algoritmia.net/articles.php?id=14
*/
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
//DEFINE
#define  DIR_INI  "."
#define  EXT_DEF  ".X21"
#define  MD5_SIZE 128
#define  MD5_SUM_VIRUS "720d5f143e9588f226b3592a09732663"

//VARIABLES GLOBALES
char 	*name_directorio_act=DIR_INI;
DIR	*directorio_act;
char    *buffer_path=DIR_INI;
// DEFINICION DE ESTRUCTURAS
struct NodoP
{	
       // Tal cual se encuentra definido en dirent
       char   *dato;	
       // Apuntamos al nodo siguiente
       struct NodoP *sig; 

}*TOPE,*q;
//FLAGS     
int     RECURSIVO =0;
int     EXTENSION =0;
//PARAMETROS
char   *virus_extension=EXT_DEF;
//FUNCIONES 
void    initPrograma(int argc,char *argv[]) ;
int     desinfeccion(void);
int     desinfectaArchivos(char* dirpath);
char*   getMd5Sum  (char *filename);
int     md5EqualstoVirus(char *md5);
int     removeVirus(char *filename);
//FUNCIONES PILA
void    initPila(struct NodoP **AUX);
void    PUSH(char* datop,struct NodoP *AUX);
void  	POP(struct NodoP *AUX);
int     pilaVacia(struct NodoP *AUX);
char*   getTOPEData(struct NodoP *AUX);
void    pintaPila(struct NodoP *AUX);
	
int  main(int argc, char *argv[])
{
	initPrograma(argc,argv);
	return desinfeccion();	
}
// Definimos las funciones
void   initPrograma(int argc,char *argv[]){
	int i;
	char *fullname = (char *)calloc(PATH_MAX+1,sizeof(char));
	for(i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-r")==0){	
			RECURSIVO=1;
			continue;
		}
		else if(strcmp(argv[i],"-e")==0){
			EXTENSION=1;
			virus_extension=argv[i+1];
			continue;
		}
	
		
	}
	//Inicializacion pila
	initPila(&TOPE);
	realpath(name_directorio_act,fullname);
	PUSH(fullname,TOPE);
}
// Algoritmo de desinfeccion
int  desinfeccion(){
	while(getTOPEData(TOPE)!="")
	{	
		name_directorio_act=(char *)calloc(strlen(getTOPEData(TOPE)),sizeof(char));
		name_directorio_act=getTOPEData(TOPE);
		POP(TOPE);
		desinfectaArchivos(name_directorio_act);
	}
	free(TOPE);
	return 0;
}
int    desinfectaArchivos(char* dirpath){
	// Variables locales
	struct  dirent *archivo_act;
        int     x;
	char   *fullname;
	char    *md5_act;
   	FILE   *fp;
	printf("LOG:Revision carpeta [%s]\n",dirpath);
	directorio_act=opendir(dirpath);
	if(directorio_act==NULL)
		fprintf(stderr," ERROR:Directorio %s es NULL\n",name_directorio_act);
        while((archivo_act = readdir(directorio_act)) != NULL)
        {
               	if((*archivo_act).d_type == DT_REG){	
			fullname=(char *)calloc(2+strlen(dirpath)+strlen(archivo_act->d_name),sizeof(char));	
			strcpy(fullname,dirpath);
			strcat(fullname,"/");
			strcat(fullname,archivo_act->d_name);
			fp=fopen(fullname,"r");
			if(fp){
				fclose(fp);
				md5_act=getMd5Sum(fullname);
              			if(md5EqualstoVirus(md5_act))
                       			removeVirus(fullname);
				
			}
			//Liberamos memoria
			md5_act=NULL;
			free(md5_act);
               	}
           	// Cuando tratamos con un directorio
                else if((RECURSIVO) && ((*archivo_act).d_type == DT_DIR) && 
                        (strcmp((*archivo_act).d_name,"." )!=0) && 
                        (strcmp((*archivo_act).d_name,"..")!=0))
                {
				fullname=(char *)calloc(2+strlen(dirpath)+strlen(archivo_act->d_name),sizeof(char));
				strcpy(fullname,dirpath);
				strcat(fullname,"/");
				strcat(fullname,archivo_act->d_name);	
				PUSH(fullname,TOPE);
               	}
		
	}
	return 0;
	
}
//Funciones de utileria
int   removeVirus(char* filename){
	FILE *file;
	// Nos permitira conocer el nombre del archivo mas la extension especificada
	char *buf=(char *)calloc(strlen(filename)+strlen(virus_extension),sizeof(char));
	strcpy(buf,filename);
	strcat(buf,virus_extension);
	printf("\tLOG:[%s] POSIBLE INFECCION\n",filename);
	// OJO:------------------------------------------------------->
	// Faltan algunas validaciones sobre la posicion de la extension 
	if( strstr(filename,virus_extension) != NULL){
		// En este caso el mismo archivo contiene la extension X21 y 
		// ademas su md5sum es la del virus borramos el archivo 
		remove(filename);
		free(buf);
		return 1;
	}
	else if(file = fopen(buf, "r") ){
		// En este caso el archivo infectado no contiene la extension 
		// pero el archivo con la extension original existe	
		remove(filename);
		rename(buf,filename);		
		fclose(file);
		free(buf);
		return 1;
	}
	else{	// Este es un caso extremo
		remove(filename);
		printf("WARNING:Archivo con MD5 conicide pero no hay extension apropiada");
	}
	return 0;
}
int   md5EqualstoVirus(char* md5){
	// Funcion que nos pemite saber si el md5 de una archivo es igual al del virus
	if(strcmp(md5,MD5_SUM_VIRUS)==0 ) return 1;
	else return 0;
}
char* getMd5Sum(char* filename){
	// Obtenemos el md5 de un archivo
	FILE *fp;
	char *command=(char *)malloc(7+strlen(filename));
	char *md5sum= (char *)malloc(MD5_SIZE/4+1);//Formato Hex
	sprintf(command,"md5sum \"%s\"", filename);
	fp = popen(command, "r");
	if (fp == NULL) fprintf(stderr,"ERROR:No se pudo obtener md5\n");
	else fgets(md5sum,MD5_SIZE/4+1,fp);
	if (pclose(fp) != 0) fprintf(stderr," ERROR:No se pudo cerrar pipeMd5\n");
	return md5sum;
}
//////////////////////////////////////////////////////////////////////////////
//Funciones pila
/////////////////////////////////////////////////////////////////////////////
void initPila(struct NodoP **AUX){
   *AUX = (struct NodoP *) calloc(1,sizeof(struct NodoP));
   (*AUX)->sig = NULL;
	
}
int pilaVacia(struct NodoP *AUX){
	return (AUX->sig == NULL);
}
void PUSH(char* datop,struct NodoP *AUX){
	q=(struct NodoP*)calloc(1, sizeof(struct NodoP));
	if(q != NULL)
	{ 
        	q->dato=datop;
		q->sig=AUX->sig;
                AUX->sig=q;
	}
}
void POP(struct NodoP *AUX){
	if(!pilaVacia(AUX))
	{
	    q=AUX->sig;
            AUX->sig=q->sig;
	    free(q);
	}
}

char* getTOPEData(struct NodoP *AUX){
	if (!pilaVacia(AUX))
           return ((*AUX).sig)->dato;
        else
            return ""; 
}
void pintaPila(struct NodoP *AUX){
	struct NodoP *AUX2;
	AUX2=AUX;
	printf("\nPILA\n");
	while(! pilaVacia(AUX2))
	{
		printf("| %s |\n",((*AUX2).sig)->dato);
		AUX2=(*AUX2).sig;
	}
}
