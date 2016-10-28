/*Elaborado por : Carlos Ruiz  Aguilar y Genaro Marmolejo Servin*/*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*DEFINICIONES UTILES*/***
#define INICIAL         0
#define NUM_ESTADOS     21
#define C_IDENTIFICADOR 0
#define C_OPREL         1
#define C_OPLOG         2
#define C_OPBIT         3
#define C_ENTO          4
#define C_ENTH          5
#define C_ENTD          6
#define C_PALR          7
#define C_OPASG         8
#define C_SIMESP        9
#define C_COMENTARIO   -1
/*DEFINICION DE VARIABLES GLOBALES o*no es/ */                           
static FILE *AIN,*TS,*TTOKENS,*LOG_ERROR;
static char  c, *buffer,*TOPREL[]={">",">=","<","<=","==","!="};
static int   INTERRUMPE,NUMID=-1,LINEA=0,NC=0; 
static int   TOPLOG[3],TOPBIT[6],TOPASG[5];    /*Tablas*/
/*Estructura 



	Usada*/
static struct Estado
{
       int ID;
       int FINAL,CLASE;              
}EstadoActual, AFD[NUM_ESTADOS]  ;
/*Inicializacion de AFD*/
void InicializaAFD()
{
     int i;
     /*ASIGNAMOS SI ES ESTADO FINAL O NO*/
     for(i=0;i<NUM_ESTADOS;i++)
     {
       AFD[i]. ID=i;
       if (i==14 || i==17 || i==19||i==0)
           AFD[i]. FINAL=0;
       else
           AFD[i]. FINAL=1;  
     }
     /*ASIGNACION DE CLASE*/
     AFD[10]. CLASE=C_IDENTIFICADOR;
     AFD[5].CLASE=AFD[6].CLASE=AFD[8].CLASE=C_OPREL;
     AFD[3].CLASE=AFD[9].CLASE=C_OPLOG;
     AFD[1].CLASE=AFD[2].CLASE=AFD[4].CLASE=C_OPBIT;
     AFD[13].CLASE=C_ENTO;
     AFD[15].CLASE=C_ENTH;
     AFD[11].CLASE=AFD[12].CLASE=C_ENTD;
     AFD[18].CLASE=AFD[7].CLASE=C_OPASG;
     AFD[16].CLASE=C_SIMESP;
     AFD[20].CLASE=C_COMENTARIO; /* FIN DE ASIGNACION DE CLASE */  
};
void InicializarTablas()
{
      fprintf(TS,"ID             Tipo\n"); 
     /*TABLA DE OPREL YA FUE DEFINIDA  TABLA OPERADORES LOGICOS*/
     TOPLOG[0]='&';
     TOPLOG[1]='|';
     TOPLOG[2]='!';
     /* TABLA OP BIT*/
     TOPBIT[0]='&';
     TOPBIT[1]='|';
     TOPBIT[2]='^';
     TOPBIT[3]='<';
     TOPBIT[4]='>';
     TOPBIT[5]='~';
     /* TABLA OP ASIGNACION*/
     TOPASG[0]='=';
     TOPASG[1]='*';
     TOPASG[2]='-';
     TOPASG[3]='/';
     TOPASG[4]='+';
};
int BuscaID(char *EB)
{   
    char buftmp_id[31];
    int i=-1,pos=-1;
    rewind(TS);
    fscanf(TS,"%s",buftmp_id);
    fscanf(TS,"%s",buftmp_id);
    while(!feof(TS))
    {
       i++;
       fscanf(TS,"%s",buftmp_id);
       if(strcmp(buftmp_id,EB)==0)
       {
           pos=i;
           break;
       }
    } 
    return pos;
};
void EscribeID(char *EB)
{    
    fseek(TS, 0,SEEK_END);
    fprintf(TS,"%s\n",EB); 
    NUMID++;
};
int BuscaOPREL(char *EB)
{
    int i;
    for(i=0;i<6;i++)
    {
      if (strcmp(TOPREL[i],EB)==0)
         break;
    }  
    return i; 
};
int BuscaOPLOG(int EB)
{
    int i;
    for(i=0;i<3;i++)
    {
      if (TOPLOG[i]==EB)
         break;
    }  
    return i; 
};
int BuscaOPBIT(int EB)
{
    int i;
    for(i=0;i<6;i++)
    {
      if (TOPBIT[i]==EB)
         break;
    }  
    return i; 
};
int BuscaOPASIG(int EB)
{
    int i;
    for(i=0;i<5;i++)
    {
      if (TOPASG[i]==EB)
         break;
    }  
    return i; 
};
int BuscaPALRES(char *EB)
{
    int  pos=-1;
    if(strcmp(EB,"char")==0)
                 pos=0;
    else if (strcmp(EB,"do")==0)
                 pos=1;
    else if (strcmp(EB,"for")==0)
                 pos=2;
    else if (strcmp(EB,"if")==0)
                 pos=3;
    else if (strcmp(EB,"int")==0)
                 pos=4;
    else if (strcmp(EB,"while")==0)
                 pos=5;
    return pos; 
}
int QuitarESPTABSALTO()
{
     int Valor;
     if(c=='\n' || c =='\t' || c==' '||c=='\r' )
         Valor =1;
     else
         Valor =0; 

     if (c=='\n')
        LINEA++;   
     return Valor;             
    
}; 
void error()
{
    fprintf(LOG_ERROR,"LINEA %d :<Simbolo no reconocido: %c>\n",LINEA,c);
};
void verificarNC(){
	if (NC==0) 
		fprintf(LOG_ERROR,"<INGRESO UN ARCHIVO VACIO NO SE GENERARON TOKENS NI SIMBOLOS>\n");
};
/* Fin de Inicializacion AFD*/
void ALex()
{
     int i=0,val,auxINT;
     while(c!=EOF)
     { 
      
       buffer = (char*)malloc( sizeof(char));
       EstadoActual.ID=INICIAL;
       INTERRUMPE=0;
       while(!INTERRUMPE)
       {
                         switch(EstadoActual. ID)
                         {
                          case 0:
                               if (c=='@')
                                    EstadoActual. ID=19;
                               else if (c=='*'||c=='-'||c=='/'||c=='+')
                                    EstadoActual.ID=17;
                               else if (c=='('||c==')'||c=='{'||c=='}'||c==';'||c==',')
                                    EstadoActual.ID=16;
                               else if ((c>64 && c<91)||(c>96 && c<123)||c=='_')
                                    EstadoActual.ID=10;
                               else if (c=='&')
                                    EstadoActual. ID=1;
                               else if (c=='&')
                                    EstadoActual. ID=1;
                               else if (c=='|')
                                    EstadoActual. ID=2;
                               else if (c=='!')
                                    EstadoActual. ID=9;
                               else if (c=='^'||c=='~')
                                    EstadoActual. ID=4;
                               else if (c=='<')
                                    EstadoActual. ID=5;
                               else if (c=='>')
                                    EstadoActual. ID=6;
                               else if (c=='=')
                                    EstadoActual. ID=7;
                               else if (c>48&&c<58)
                                    EstadoActual. ID=11;
                               else if (c=='0')
                                    EstadoActual. ID=12;
                               else
                                   INTERRUMPE=1;
                               break;
                          case 1:
                               if (c=='&')
                                    EstadoActual. ID=3;
                               else
                                   INTERRUMPE=1;
                               break;
                          case 2:
                               if (c=='|')
                                    EstadoActual. ID=3;
                               else
                                   INTERRUMPE=1;
                               break;
                          case 3:
                               INTERRUMPE=1;
                               break;
                          case 4:
                               INTERRUMPE=1;
                               break;
                          case 5:
                               if (c=='<')
                                    EstadoActual. ID=4;
                               else if (c=='=')
                                    EstadoActual. ID=8;
                               else
                                   INTERRUMPE=1;
                               break;
                          case 6:
                               if (c=='>')
                                    EstadoActual. ID=4;
                               else if (c=='=')
                                    EstadoActual. ID=8;
                               else
                                   INTERRUMPE=1;
                               break;
                          case 7:
                               if (c=='=')
                                    EstadoActual. ID=8;
                               else
                                   INTERRUMPE=1;
                               break;
                               break;
                          case 8:
                               INTERRUMPE=1;
                               break;
                          case 9:
                               if (c=='=')
                                    EstadoActual. ID=8;
                               else
                                   INTERRUMPE=1;
                               break;
                          case 10:
                               if ((c>64 && c<91)||c>47&&c<58||(c>96 && c<123)||c=='_')
                                    EstadoActual.ID=10;
                               else
                                   INTERRUMPE=1;
                               break;
                          case 11:
                               if (c>47&&c<58)
                                    EstadoActual.ID=11;
                               else
                                   INTERRUMPE=1;
                               break;
                          case 12:
                                if (c>47&&c<56)
                                    EstadoActual.ID=13;
                                else if (c=='x'||c=='X')
                                    EstadoActual.ID=14;
                                else
                                   INTERRUMPE=1;
                               break;
                          case 13:
                                if (c>47&&c<56)
                                    EstadoActual.ID=13;
                                else
                                   INTERRUMPE=1;
                               break;
                          case 14:
                               if ((c>64 && c<71)||c>47&&c<58||(c>96 && c<103))
                                    EstadoActual.ID=15;
                               else
                                   INTERRUMPE=1;
                               break;
                          case 15:
                               if ((c>64 && c<71)||c>47&&c<58||(c>96 && c<103))
                                    EstadoActual.ID=15;
                               else
                                   INTERRUMPE=1;
                               break;
                          case 16:
                               INTERRUMPE=1;
                               break;
                          case 17:
                               if (c=='=')
                                    EstadoActual.ID=18;
                                else
                                   INTERRUMPE=1;
                               break;
                          case 18:
                               INTERRUMPE=1;
                               break;
                          case 19:
                               if (!(c=='\n'))
                                    EstadoActual.ID=19;
                                else if (c=='\n')
                                     EstadoActual.ID=20;
                                else
                                   INTERRUMPE=1;
                               break;
                          case 20:
                               INTERRUMPE=1;
                               break;
                         } 
                                         
                         /*Almacenamos Dato en el Buffer Lectura del Sigu caracter*/
                         if (!INTERRUMPE)
                         {
                            buffer[i] = c; 
                            i++;
                            buffer=(char*)realloc(buffer, (i+1)*sizeof(char));
                            c=fgetc(AIN);
			                NC++;
                         }
      }  
      buffer[i]='\0'; 
      if (AFD[EstadoActual. ID].FINAL==1 )
         {
                     
                    if (AFD[EstadoActual. ID].CLASE==C_IDENTIFICADOR)
                    {
                       val=BuscaPALRES(buffer);
                       if(val==-1)
                       {
                             val=BuscaID(buffer);/* Buscamos la posicion del identificador*/
                              if (strlen(buffer)>31)
                                      buffer[31]='\0';
                             if(val==-1)         /*Si aun no existe en la tabla de simbolos*/
                             {
                                       EscribeID(buffer);
                                       val=NUMID;
                             }
                             fprintf(TTOKENS,"(%d,%d)\n",C_IDENTIFICADOR,val,buffer);
                       }
                       else
                         fprintf(TTOKENS,"(%d,%d)\n",C_PALR,val,buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_OPREL)
                    {
                       val=BuscaOPREL(buffer);
                       fprintf(TTOKENS,"(%d,%d)\n",C_OPREL,val,buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_OPLOG)
                    {
                       val=BuscaOPLOG(buffer[0]);
                       fprintf(TTOKENS,"(%d,%d)\n",C_OPLOG,val,buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_OPBIT)
                    {
                       val=BuscaOPBIT(buffer[0]);  
                       fprintf(TTOKENS,"(%d,%d)\n",C_OPBIT,val,buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_ENTO)
                    {
                       sscanf(buffer,"%i",&auxINT);
                       fprintf(TTOKENS,"(%d,%d)\n",C_ENTO,auxINT,buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_ENTH)
                    {
                       sscanf(buffer,"%i",&auxINT);
                       fprintf(TTOKENS,"(%d,%d)\n",C_ENTH,auxINT,buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_ENTD)
                    {
                       fprintf(TTOKENS,"(%d,%d)\n",C_ENTD,atoi(buffer),buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_OPASG)
                    {
                       val=BuscaOPASIG(buffer[0]);
                      fprintf(TTOKENS,"(%d,%d)\n",C_OPASG,val,buffer);
                    }                    
                    else if (AFD[EstadoActual. ID].CLASE==C_SIMESP)
                    {
                       fprintf(TTOKENS,"(%d,%d)\n",C_SIMESP,buffer[0],buffer);
                    }
                   
       
          }
      else
          {
                    if (!QuitarESPTABSALTO())
                                     error();
                    c=fgetc(AIN);
	       	        NC++;
          }
      /*Liberar Memoria de Buffer*/
       i=0;
       buffer =NULL;
       free(buffer); 
      /*Fin de Liberacion de Memoria de Buffer*/
    } 
    
};
   
int main (int argc , char **argv )
{   

    /* ABRIMOS ARCHIVOS*/
    AIN=fopen(argv[1],"r");
    if (AIN==NULL)
       printf("No existe el Archivo especificado\n");
    else
    {
        TTOKENS=fopen("TOKENS.txt","w");
        TS=fopen("TS.txt","w+r");
        LOG_ERROR=fopen("LOG_ERRORES.txt","w+r");
        /*Inicializamos tablas*/
        InicializaAFD();
        InicializarTablas();
        /*Analisis Lexico*/
        c=fgetc(AIN);
        ALex();
	verificarNC();
        printf("El analisis lexico ha culminado!! Los archivos generados son :\n\t1)TOKENS.txt\n\t2)TS.txt\n\t3)LOG_ERRORES.txt\n");
        /*Cerramos archivo*/
        fclose(AIN);
        fclose(TTOKENS);
        fclose(TS);
        fclose(LOG_ERROR);
    }
    return 0;
}
