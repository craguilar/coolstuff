/*Elaborado por : Carlos Ruiz  Aguilar y Genaro Marmolejo Servin*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*DEFINICIONES UTILES ANALIZADOR LEXICO*/
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
/*DEFINICIONES  ANALIZADOR SINTACTICO*/
#define RE_C           0
#define RE_I           1
#define RE_P           2
/*ACCIONES   */
#define ASIGNA_TIPO    0
#define QCAR           1
#define PRINT_WHILE    2
#define PRINT_PAR      3
#define SET_ONINC      4
#define SET_OFFINC     5
#define PRINT_BUFFINC  6  
#define PRINT_CIERRE   7
/*TAMANO DE BUFFER DE USO EN TRADUCCION*/

/*DEFINICION DE VARIABLES GLOBALES */                           
static FILE *AIN,*TS,*TTOKENS,*LOG_ERROR;
/*Uso sintactico eas es la entrada del analizador sintactico*/
static char  *eas;
/*Cadena traducida*/
static char  *cad_trad;
static char  buff_INC[30][100];
static int   n_inc,nfor=-1;
/*Variable que controla el flujo de la cadena traducida*/
static int   SET_INC=0;    
/*Tablas*/
static char  *TOPREL[]={">",">=","<","<=","==","!="}; 
static int   TOPLOG[3],TOPBIT[6],TOPASG[5];    
/*Variables de uso general*/
int ERROR_G=0;
char  c;
/*Se usa para llevar el control de el buffer eas*/
int j=0;
/*Se usa para llevar el control de el buffer cad_trad*/
int n_cd=0;
/*Se usa para llevar el control de la estructura de la Tabla de Simbolos*/
int ID_ACTUAL=0;
/*Se usa para llenar la Tabla de Simbolos*/
int FT_AT=0;
/* ASin*/
int ERROR_S=0,EP=0,MP=0,NES=0,c_ant;
char *palabraini;
/*Variable de uso global ya que nos permite una manimulacion directa de la 
asignacion de tipos*/
char att_tipo,att;

/*Variable de uso global ya que cuando ocurre un error de un identificador
 no declarado usamos la misma funcion que los errores semanticos pasando como 
 nulo el parametro este valor guarda la posicion del  identificador no declarado*/
int ULT_POS,att_pos;
/* ALex.c*/
static int   INTERRUMPE, NUMID=-1,LINEA=1, NC=0;
/*Estructuras Usadas*/
static struct Estado
{
       int ID;
       int FINAL,CLASE;              
}EstadoActual, AFD[NUM_ESTADOS]  ;
/*Tabla de Simbolos Temporal*/
static struct ID_TIPO
{
       char *ID;
       int tipo;
}*TS_TEMP;
/*Usada en la pila*/
struct NodoP
{
       /*Dato almacena el valor de un caracter y pos almacena el valor del 
       atributo posicion  , esto es ineficiente sin embargo para efectos practicos
       funciona
       NUM_ACCION:USADO PARA INDICAR EL SIMBOLO DE ACCION*/
       char dato;
       int  pos ;
       int  NUM_ACCION;
       /*Nodo Siguiente*/
       struct NodoP *sig; 
       /*USADO PARA APUNTAR OTROS NODOS CUANDO SE HACE REFERENCIAS A ATRIBUTOS*/
       struct NodoP *REF ;
                   
}*TOPE,*q,AUXNP,*AUXANP;
/* Manejo de Errores General*/
void F_ERROR(int TE){
     switch(TE)
     {
               case 1:
                    printf("\n\t2)Han OCURRIDO errores  en el ANALISIS LEXICO.VER LOG_EVENTOS.txt\n");
                    break;
               case 2:
                    printf("\n\t2)Han OCURRIDO %d errores  en el ANALISIS SINTACTICO.VER LOG_EVENTOS.txt\n",NES);
                    fprintf(LOG_ERROR,"\n\t::Han OCURRIDO %d errores  en el ANALISIS SINTACTICO.Cadena NO valida.\n",NES);
                    break;
     }
    
};
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
     TOPASG[1]='+';
     TOPASG[2]='-';
     TOPASG[3]='*';
     TOPASG[4]='/';
};
int BuscaID(char *EB)
{   
    int i=-1,pos=-1;
    for(i=0;i<ID_ACTUAL;i++)
    {
                            if(strcmp(TS_TEMP[i].ID,EB)==0)
                            {
                                                                   pos=i;
                                                                   break;
                            }
    }
    return pos;
};
void SetAtt(char v)
{

     char  valor[100],k;
     /*$*/
                                          j++;
                                          eas=(char*)realloc(eas,(j+1)*sizeof(char));
                                          eas[j]='$'; 
   
                                           j++;
                                            eas=(char*)realloc(eas,(j+1)*sizeof(char));
                                            eas[j]=v;                                                         
     
     /*$*/
                                          j++;
                                          eas=(char*)realloc(eas,(j+1)*sizeof(char));
                                          eas[j]='$'; 
     
};

void SetAttPosicion(int v)
{

     char  valor[100],k;
     /*$*/
                                          j++;
                                          eas=(char*)realloc(eas,(j+1)*sizeof(char));
                                          eas[j]='$'; 
     sprintf(valor, "%d", v);
     //itoa(v,valor,10);
     for(k=0;k<strlen(valor);k++)
     {
                                    j++;
                                    eas=(char*)realloc(eas,(j+1)*sizeof(char));
                                    eas[j]=valor[k];                                                         
     }
     /*$*/
                                          j++;
                                          eas=(char*)realloc(eas,(j+1)*sizeof(char));
                                          eas[j]='$'; 
     
};
void EscribeID(char *EB)
{    
    /*
    Hecho para la primera version del programa
          fseek(TS, 0,SEEK_END);
          fprintf(TS,"%s\n",EB);
    */
    TS_TEMP=(struct ID_TIPO*)realloc(TS_TEMP,(ID_ACTUAL+1)*sizeof(struct ID_TIPO));
    TS_TEMP[ID_ACTUAL].ID=EB;
    TS_TEMP[ID_ACTUAL].tipo=-1;
    ID_ACTUAL++;                      
    NUMID++;
};
void PINTA_TS()
{
      char buftmp_id[40],car;
      int i=0;
      if(FT_AT==0)
      {
       //UNICAMENTE SE HACE UNA VEZ queremos leer el encabezado del archivo
       rewind(TS);
       
       fscanf(TS,"%s",buftmp_id);
       fscanf(TS,"%s",buftmp_id);
       FT_AT=1;
       // APUNTAMOS AL IDENTIFICADOR ACTIAL     
      }

      for(i=0;i<ID_ACTUAL;i++)
      {
      /*USAMOS ESTO PARA UBICARNOS EN ELPUNTO ACTUAL*/
                fseek(TS, 0,SEEK_END);
                if (TS_TEMP[i].tipo==0)      fprintf(TS,"%s  char\n",TS_TEMP[i].ID);
                else if (TS_TEMP[i].tipo==4) fprintf(TS,"%s  integer\n",TS_TEMP[i].ID);
                else                         fprintf(TS,"%s  DESCONOCIDO\n",TS_TEMP[i].ID);
      }
      /*$*/
                                          j++;
                                          eas=(char*)realloc(eas,(j+1)*sizeof(char));
                                          eas[j]='$'; 
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
char GetAtomoOPREL(int i)
{
     char aux;
     switch(i)
     {
              case 0:aux='>';
                   break;
              case 1:aux='g';
                   break;
              case 2:aux='<';
                   break;
              case 3:aux='l';
                   break;
              case 4:aux='e';
                   break;
              case 5:aux='n';
                   break;
     }
     return aux;
     
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
char GetAtomoOPLOG(int i)
{   char aux;
    switch(i)
    {
             case 0:aux='y';
                  break;
             case 1:aux='r';
                  break;
             case 2:aux='!';
                  break;
    }
    return aux; 
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
char GetAtomoOPBIT(int i)
{   char aux;
    switch(i)
    {
             case  0:aux='&';
                   break;
             case  1:aux='|';
                   break;
             case  2:aux='^';
                   break;
             case  3:aux='s';
                   break;
             case  4:aux='z';
                   break;
             case  5:aux='~';
                   break;
    }
    return aux; 
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
char GetAtomoOPASIG(int i)
{
     char aux;
     switch(i)
     {
              case 0:aux='=';
                   break;
              case 1:aux='+';
                   break;
              case 2:aux='-';
                   break;
              case 3:aux='*';
                   break;
              case 4:aux='/';
                   break;
     }
     return aux;
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
};
char GetAtomoPALRES(i)
{
    char aux;
    
    if(i==0)          aux='c';
    else if (i==1)    aux='d';
    else if (i==2)    aux='f';
    else if (i==3)    aux='b';
    else if (i==4)    aux='i';
    else if (i==5)    aux='w';
    return aux; 
};
int  TipodeDato(i)
{    
    if(i==0 || i==4)          return 1;
    else                      return 0;
};
char  GetTipoDato(i)
{    
    if(i==0)          return 'c';
    else              return 'i';
};
int QuitarESPTABSALTO()
{
     int Valor;
     if(c=='\n' || c =='\t' || c==' '||c=='\r' )
         Valor =1;
     else
         Valor =0; 

     if (c=='\n')
     {
        LINEA++;  
        eas[j]='_';
        j++;
        eas=(char*)realloc(eas,(j+1)*sizeof(char));
     } 
     return Valor;             
    
}; 
/* Manejo de errores Analizador Lexico*/
void errorLex(char *buf)
{
    ERROR_G=1;
    printf("\nERROR LEXICO LINEA %d :<Simbolo no reconocido: %s%c>\n",LINEA,buf,c);
    fprintf(LOG_ERROR,"LINEA %d :<Simbolo no reconocido: %s%c>\n",LINEA,buf,c);
};
void verificarNC(){
	if (NC==0) 
		fprintf(LOG_ERROR,"\t<INGRESO UN ARCHIVO VACIO NO SE GENERARON TOKENS NI SIMBOLOS>\n");
};
/* Fin de Inicializacion AFD*/
void ALex()
{
     static char *buffer;
     eas    = (char*)malloc( sizeof(char));
     /* i es usadda con el buffer del analisis 
     lexico y  j con la cadena de entrada
     al analizados sintactico*/
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
                             /*Colocamos su atributo en este caso es la: posicion*/
                             eas[j] ='a';             
                             SetAttPosicion(val);
                             /*FIN DE COLOCAR  POSICION*/
                            
                       }
                       else
                       {
                         eas[j]= GetAtomoPALRES(val);
                         SetAtt(GetAtomoPALRES(val));
                         fprintf(TTOKENS,"(%d,%d)\n",C_PALR,val,buffer);
                         
                       }
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_OPREL)
                    {
                       val=BuscaOPREL(buffer);
                       eas[j]= GetAtomoOPREL(val);
                       SetAtt(GetAtomoOPREL(val));
                       fprintf(TTOKENS,"(%d,%d)\n",C_OPREL,val,buffer);
                       
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_OPLOG)
                    {
                       val=BuscaOPLOG(buffer[0]);
                       eas[j] =GetAtomoOPLOG(val);
                       SetAtt(GetAtomoOPLOG(val));
                       fprintf(TTOKENS,"(%d,%d)\n",C_OPLOG,val,buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_OPBIT)
                    {
                       val=BuscaOPBIT(buffer[0]);  
                       eas[j]=GetAtomoOPBIT(val);
                       SetAtt(GetAtomoOPBIT(val));
                       fprintf(TTOKENS,"(%d,%d)\n",C_OPBIT,val,buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_ENTO)
                    {
                       sscanf(buffer,"%i",&auxINT);
                       eas[j]='o';
                       SetAtt('o');
                       fprintf(TTOKENS,"(%d,%d)\n",C_ENTO,auxINT,buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_ENTH)
                    {
                       sscanf(buffer,"%i",&auxINT);
                       eas[j]='h';
                       SetAtt('h');
                       fprintf(TTOKENS,"(%d,%d)\n",C_ENTH,auxINT,buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_ENTD)
                    {
                       eas[j]='x';
                       SetAtt('x');
                       fprintf(TTOKENS,"(%d,%d)\n",C_ENTD,atoi(buffer),buffer);
                    }
                    else if (AFD[EstadoActual. ID].CLASE==C_OPASG)
                    {
                      val=BuscaOPASIG(buffer[0]);
                      eas[j]=GetAtomoOPASIG(val);
                      SetAtt(GetAtomoOPASIG(val));
                      fprintf(TTOKENS,"(%d,%d)\n",C_OPASG,val,buffer);
                    }                    
                    else if (AFD[EstadoActual. ID].CLASE==C_SIMESP)
                    {
                       eas[j]=buffer[0];
                       SetAtt(buffer[0]);
                       fprintf(TTOKENS,"(%d,%d)\n",C_SIMESP,buffer[0],buffer);
                    }
		            else if (AFD[EstadoActual. ID].CLASE==C_COMENTARIO)
                    {
              			/*NO nos interesa hacer nada
                                   printf("\n ES UN COMMENT:%s",buffer);
            			*/
            			eas[j]='_';
                    }
                                                          
                   	 j++;
                    	 eas=(char*)realloc(eas,(j+1)*sizeof(char));
		    
                    
          }
          else
          {
                    if (!QuitarESPTABSALTO())
                                     errorLex(buffer);
                    c=fgetc(AIN);
	       	        NC++;
          }
      /*Liberar Memoria de Buffer*/
       i=0;
       buffer =NULL;
       free(buffer); 
      /*Fin de Liberacion de Memoria de Buffer*/
    } 
    printf("\n\n:::Resultado del Analisis Lexico:\n");
    if(ERROR_G==0)
    {
         printf("\n\t1)El analisis lexico ha culminado!! Los archivos generados son :\n\t\ta)TOKENS.txt\n\t\tb)TS.txt\n\t\tc)LOG_ERRORES.txt\n");
         fprintf(LOG_ERROR,"\n\t No hubo errores en el analisis lexico.");
    }
    else 
        F_ERROR(ERROR_G);
};
/*Estructura de Pila*/
void  CreaPila()
{
      TOPE=NULL;
};
void  InicializaPila()
{
      TOPE=NULL;
};
void PUSH(char dato)
{       
        q=(struct NodoP*)malloc( sizeof(struct NodoP));
        (*q).dato=dato;
        (*q).sig=TOPE;
        TOPE=q;
        q =NULL;
        free(q);
        MP=0;
};
void POP()
{  
        if(TOPE!=NULL)       
            TOPE=(*TOPE).sig;
        else
            EP=1; 
        MP=0;
                
};
void REEM_R(char buf[])
{
        int k;
        POP();
       if (EP==0)
       {
          for(k=0;k<strlen(buf);k++)
          {
            PUSH(buf[strlen(buf)-1-k]);                     
          }
       }

        
};
/*
Esta funcion obtiene  la posicion de memoria de un elemento de la PILA*/
struct NodoP* GetTope_n(int n)
{
     int i;
     struct NodoP *AUX;
     AUX=TOPE;
     for(i=0;i<=n;i++)
     {
             AUX=(*AUX).sig;
     }
     if(AUX!=NULL)
                   return AUX;
     else 
                   return NULL;
     
};

char GetTope()
{       
        if (TOPE!=NULL)
           return (*TOPE).dato;
        else
            return '\0';
}; 
/*Funcion que permite conocer el contenido de la pila*/
void  PINTA_PILA()
{
     int i;
     struct NodoP *AUX;
     AUX=TOPE;
     printf("\nPILA\n");
     while( AUX!=NULL)
     {
             printf("| %c |[pos]=%d\n",(*AUX).dato,(*AUX).pos);
             AUX=(*AUX).sig;
             
     }
     
     
};
void  AVANZA()

{        
        int k;
        k=j;
        c_ant=eas[k]; 
        while(c_ant=='_')
        {
            k--;
	    if (k>-1)
            	c_ant=eas[k];
            
        }
        j++;
        c=eas[j];
        while(c=='_')
        {
           AVANZA();
            LINEA++;
        }
};  

void  RETEN()
{       
        c=eas[j];
        MP=0;
};
void InicializaCadena()
{
     eas[j]='\0';
     j=0;
     c=eas[j];
     while(c=='_')
     {
           AVANZA();
           LINEA++;
     } 
};
/*
Funciones que obtienen un atributo ya sea de posicion o de Tipo de la cadena de 
atomos
*/
int GetAttributoPos()
{
     char  v[100],value;
     int    k=0;
     /*v    = (char*)malloc( sizeof(char));*/
     AVANZA();//ESTAMOS APUNTANDO  a $
     AVANZA();//APUNTAMOS AL PRIMER DIGITO DE LA POSICION
     value=eas[j];
  
     while(value !='$')
     {
                 v[k]=value;
                 k++;	
                 AVANZA();
                 value=eas[j];
		
     }
     return atoi(v);      
}; 
char GetAttributo_ATOMO()
{
     char v;
     AVANZA();//ESTAMOS APUNTANDO  a $
     AVANZA();//Estamos apuntadno al valor deseado
     v=eas[j];
     AVANZA();//ESTAMOS APUNTANDO A %
     return v;
}; 

/*Funcion de imprimir */
void  PRINT_ID(int  pos)
{
   if(SET_INC==0)
   {
      cad_trad[n_cd]='a';
      n_cd++;
      cad_trad=(char*)realloc(cad_trad,(n_cd+1)*sizeof(char));
   }
   else
   {
      buff_INC[nfor][n_inc]='a';
      n_inc++;
   }
      
      
};
/*Funcion de imprimir*/
void  PRINT_C(char car)
{
   if(SET_INC==0)
   {
      cad_trad[n_cd]=car;
      n_cd++;
      cad_trad=(char*)realloc(cad_trad,(n_cd+1)*sizeof(char));
   }
   else
   {
      buff_INC[nfor][n_inc]=car;
      n_inc++;       
   }
      

};
void COPY_BUFFINC_CADTRAD()
{
     char copy,kinc=0;
     
     copy=buff_INC[nfor][kinc];
     
     while(copy!='\0')
     {
                 cad_trad[n_cd]=copy;
                 n_cd++;
                 cad_trad=(char*)realloc(cad_trad,(n_cd+1)*sizeof(char));
                 
                 kinc++;
                 copy=buff_INC[nfor][kinc];     
     }
     nfor--;
   
    
};
void SET_OFF_INCREMENTO()
{
     buff_INC[nfor][n_inc]='\0';
     SET_INC=0;
     
};
void SET_INCREMENTO()
{
     /*buff_INC[nfor][n_inc]*/
     nfor++;
     n_inc=0;
     SET_INC=1;
};
void  QUITA_CAR()
{
      /*
      n_cd=0 [ ]
              0
              ^
      PRINT_C
             [i]
              0
      n_cd=1 [i][ ]
              0  1
                 ^
      PRINT_C
             [i][i]
              0  1
      n_cd=2 [i][i][ ]
              0  1  2
                    ^
      
      QUITA_CAR
      
      n_cd=1 [i][i][ ]
              0  1  2
                 ^
             [i][i]
              0  1  
                 ^
      */
      n_cd--;
      cad_trad=(char*)realloc(cad_trad,(n_cd+1)*sizeof(char));
};
void PRINT_CIERRE_W()
{
     cad_trad[n_cd]=';';
     n_cd++;
     cad_trad=(char*)realloc(cad_trad,(n_cd+1)*sizeof(char));
     
     cad_trad[n_cd]='}';
     n_cd++;
     cad_trad=(char*)realloc(cad_trad,(n_cd+1)*sizeof(char));
};
void PRINT_PARENTESIS()
{
     cad_trad[n_cd]=')';
     n_cd++;
     cad_trad=(char*)realloc(cad_trad,(n_cd+1)*sizeof(char));
     cad_trad[n_cd]='{';
     n_cd++;
     cad_trad=(char*)realloc(cad_trad,(n_cd+1)*sizeof(char));
};
void PRINT_W()
{
     cad_trad[n_cd]='w';
     n_cd++;
     cad_trad=(char*)realloc(cad_trad,(n_cd+1)*sizeof(char));
     cad_trad[n_cd]='(';
     n_cd++;
     cad_trad=(char*)realloc(cad_trad,(n_cd+1)*sizeof(char));
};
/* Manejo de Errores analizador Sintactico*/
char* GetTipo(char ce)
{
       switch(ce)
       {
                    case 'a':
                         return "identificador.";
                         break;
                    case 'x':
                         return "entero decimal.";
                         break;
                    case 'o':
                         return "entero octal.";
                         break;
                    case 'h':
                         return "entero hexadecimal.";
                         break;
                    case 'c':
                         return "char.";
                         break;
                    case 'd':
                         return "do.";
                         break;
                    case 'f':
                         return "for.";
                         break;
                    case 'b':
                         return "if.";
                         break;
                    case 'i':
                         return "int.";
                         break;
                    case 'w':
                         return "while.";
                         break;
                    case 'y':
                         return "&&";
                         break;
                    case 'r':
                         return "||";
                         break;
                    case '!':
                         return "!";
                         break;
                    case  '(':
                          return "(";
                          break;
                    case ')':
                         return ")";
                         break;
                    case  '{':
                          return "{";
                          break;
                    case '}':
                         return "}";
                         break;
                    case  ';':
                          return "; ";
                          break;
                    case  ',':
                          return ", ";
                          break;
                    case '=':
                         return "=";
                         break;
                    case '+':
                         return "+";
                         break;
                    case '-':
                         return "-";
                         break;
                    case '*':
                         return "*";
                         break;
                    case '/':
                         return "/";
                         break;
                    case '>':
                         return ">";
                         break;
                    case 'g':
                         return ">=";
                         break;
                    case '<':
                         return "<";
                         break;
                    case 'l':
                         return "<=";
                         break;
                    case 'e':
                         return "==";
                         break;
                    case 'n':
                         return "!=";
                         break;
                    case '&':
                         return "&";
                         break;
                    case '|':
                         return "|";
                         break;
                    case '^':
                         return "^";
                         break;
                    case 's':
                         return "<<";
                         break;
                    case 'z':
                         return ">>";
                         break;
                    default:
            			   /*printf("\nCaracter :%c\n",ce);*/
                         return "identificador o tipo(elemento con atributo)| UNKNOWN";
                         break;
       }
}

void Recuperacione(int TE,char buf[])
{
    
     if(MP==1&&c==';')
     {
                      MP=0;
                      InicializaPila();
                      PUSH('M');
                      REEM_R("G}");
                      AVANZA();
                      printf("\t<RECUPERACION DE ERROR %d:FORZADO MODO PANICO>\n",NES);
     }
     else
     {
         
         switch(TE)
         {
                   case RE_C:
                        RETEN();
                        REEM_R(buf);
                        printf("\t<RECUPERACION DE ERROR %d: CORREGIR>\n",NES);
                        break;
                   case RE_I:
                        AVANZA();
                        POP();
                        printf("\t<RECUPERACION DE ERROR %d:IGNORAR>\n",NES);
                        break;
                   case RE_P:
                        AVANZA();
                        if (MP==0)
                           printf("\t<RECUPERACION DE ERROR %d:MODO PANICO>\n",NES);
                        MP=1;
                        break;
         }
         
     }
};
void errorS(char buf[])
{
     /*ESTE IF ESTA A PRUEBA*/
     if(MP==0)
     {
         if (buf!=NULL)
         {
             ERROR_S=1;
             NES++;
             printf("\nError Sintactico (%d):\n",NES);
             fprintf(LOG_ERROR,"\nError Sintactico (%d):\n",NES);
             fprintf(LOG_ERROR,"\tLINEA %d:Se esperaba %s despues de  %s\n\tSe encontro :%s  \n",LINEA,buf,GetTipo(c_ant),GetTipo(c));
             printf("\tLINEA %d:Se esperaba: %s despues de  %s\n",LINEA,buf,GetTipo(c_ant));
         }
         else
         {
             /*ERROR SEMANTICO IDENTIFRICADOR NO DECLARADO*/ 
             ERROR_S=1;
             NES++;
             printf("\nError Semantico (%d):\n",NES);
             fprintf(LOG_ERROR,"\nError Semantico (%d):\n",NES);
             fprintf(LOG_ERROR,"\tLINEA %d: Identificador %s no declarado.  \n",LINEA,TS_TEMP[ULT_POS]. ID);
             printf("\tLINEA %d:Identificador %s no declarado. \n",LINEA,TS_TEMP[ULT_POS]. ID);
         }
     }
};
void errorS_ID_RED()
{
     /*ESTE IF ESTA A PRUEBA*/
     if(MP==0)
     {
         /*ERROR SEMANTICO IDENTIFRICADOR NO DECLARADO*/ 
             ERROR_S=1;
             NES++;
             printf("\nError Semantico (%d):\n",NES);
             fprintf(LOG_ERROR,"\nError Semantico (%d):\n",NES);
             fprintf(LOG_ERROR,"\tLINEA %d: Identificador %s redeclarado.  \n",LINEA,TS_TEMP[ULT_POS]. ID);
             printf("\tLINEA %d:Identificador %s redeclarado. \n",LINEA,TS_TEMP[ULT_POS]. ID);
            
         
     }
};
/*REVISA TIPO*/
void RD(int pos)
{
     if(TS_TEMP[pos].tipo==-1)
     {                        
                              ULT_POS=pos;
                              errorS(NULL);
                              
     }
};
/*Funcion de asignacion de Tipo*/
 void AT(int pos,char tipo)
{
      
      
    if( TS_TEMP[pos].tipo==-1)
    { 
              if(tipo=='c')
              {
                           TS_TEMP[pos].tipo=0;
                           //fprintf(TS,"%s char\n",TS_TEMP[pos].ID);
              }
              else if(tipo=='i')
              {
                           TS_TEMP[pos].tipo=4;
                          // fprintf(TS,"%s integer\n",TS_TEMP[pos].ID);
              }
              else
              {
                  TS_TEMP[pos].tipo=-1;
              }
      
    }
    else
    {
               ULT_POS=pos;
              errorS_ID_RED();
    }
      

       
    
};
/*Asignacion de las ACCIONES
#define ASIGNA_TIPO    0
#define QCAR           1
#define PRINT_WHILE    2
#define PRINT_PAR      3
#define SET_ONINC      4
#define SET_OFFINC     5
#define PRINT_BUFFINC  6 
*/
void ACCION(int NUMACCION)
{
     int pos;
     char tipo;
     POP();
     switch(NUMACCION)
     {
                      /*ASIGNACION DE TIPO*/
                      case ASIGNA_TIPO: 
                           
                               tipo= (*TOPE).dato;
                               POP();
                               
                               pos= (*TOPE).pos;
                               POP();
                               
                               AT(pos,tipo);
                           
                           break;
                      case QCAR:
                           
                                QUITA_CAR();
                                
                           break;
                      case PRINT_WHILE:
                           
                                PRINT_W();
                                
                           break;
                      case PRINT_PAR:
                           
                                PRINT_PARENTESIS();
                                
                           break;
                      case SET_ONINC:
                           
                               SET_INCREMENTO();
                               
                           break;
                      case SET_OFFINC:
                           
                               SET_OFF_INCREMENTO();
                               
                           break;
                      case PRINT_BUFFINC:
                           
                               COPY_BUFFINC_CADTRAD();
                               
                           break;
                      case PRINT_CIERRE:
                               PRINT_CIERRE_W();
                           break;
                      default:
                              printf("ACCION NO ESPECIFICADA");
                           break;
     }
};

/*------------------------------------------*/
void ASin()
{
 int A=0;
 cad_trad=(char*)malloc( sizeof(char));

 while(c!='\0' &&EP==0) //&& ES==0)
 {
     switch(GetTope())
     {
                    		case 'M':
                                     if (c=='{')
                                     {
                                                
                                                
                                                REEM_R("DRSG}");
                                                //RECUPERACION DEL ATRINUTO/////
                                                att=GetAttributo_ATOMO();
                                                PRINT_C(att);
                                                AVANZA();
                                                ////////////////////////////////    
                                     }
                                     else
                                     {
                                         errorS("{");
                                         Recuperacione(RE_C,"DRSG}");
                                     }
                                     break;
                            case 'R':
                                     if (c=='c'||c=='i')
                                     {
                                                REEM_R("DR");
                                                RETEN();
                                     }
                                     else  if (c=='f'||c=='w'||c=='d'||c=='a'||c=='b')
                                     {
                                                POP();
                                                RETEN();
                                     }
                                     else
                                     {
                                         errorS("char int for while do identificador if");
                                         Recuperacione(RE_P,"");
                                         
                                     }
                                     break;
                    		case 'G':
                                  
                                     if (c=='f'||c=='w'||c=='d'||c=='a'||c=='b')
                                     {
                                           REEM_R("SG");
                                           RETEN();
                                     }
                                     else if(c=='}')
                                     {
                                         POP();
                                         RETEN();
     
                                     }
                                     else
                                     {
                                         errorS("for while do identificador if }");
                                         Recuperacione(RE_P,"");
                                     }
                                     break;
                    		case 'D':
                                     if (c=='c'||c=='i')
                                     {
                                     /*
                                        RETENER()
                                    	POP()
                                    	PUSH(NIL)
                                    	PUSH(L)
                                    	PUSH(NIL)
                                    	PUSH(SP-2);Indica un apuntador a la posición SP-2
                                    	PUSH({AT})
                                    	PUSH(SP-2)
                                    	PUSH(a)
                                    	PUSH(SP-3) 

                                     */
                                           POP();
                                           PUSH('-');
                                           PUSH('L');
                                           PUSH('-');
                                           /*PUSH(SP-2)*/
                                                       PUSH('-');
                                                       (*TOPE).REF=GetTope_n(2);
                                           /*FIN DE PUSH (SP-2)*/
                                           
                                            /*PUSH{AT}*/
                                                       PUSH('#');
                                                       (*TOPE).NUM_ACCION=ASIGNA_TIPO;
                                           /*FIN DE ACCION*/
                                            /*PUSH(SP-2)*/
                                                       PUSH('-');
                                                       (*TOPE).REF=GetTope_n(2);
                                           /*FIN DE PUSH (SP-2)*/
                                           PUSH('a');
                                            /*PUSH(SP-3)*/
                                                       PUSH('-');
                                                       (*TOPE).REF=GetTope_n(3);
                                           /*FIN DE PUSH (SP-3)*/
                                           PUSH('T');
                                           RETEN();
                                     }
                                     else
                                     {
                                          errorS(" char int");\
                                          /* 
                                          RECUPERACION DE ERROR CORREGIR
                                                       Recuperacione(RE_C,"aL");
                                                       RETEN();
                                                       REEM_R(buf);
                                          */
                                           POP();
                                           PUSH('@');
                                           PUSH('L');
                                           PUSH('-');
                                           /*PUSH(SP-2)*/
                                                       PUSH('@');
                                           /*FIN DE PUSH (SP-2)*/
                                           
                                            /*PUSH{AT}*/
                                                       PUSH('#');
                                                       (*TOPE).NUM_ACCION=ASIGNA_TIPO;
                                           /*FIN DE ACCION*/
                                            /*PUSH(SP-2)*/
                                                       PUSH('-');
                                                       (*TOPE).REF=GetTope_n(2);
                                           /*FIN DE PUSH (SP-2)*/
                                           PUSH('a');
                                            
                                           RETEN();
                                           printf("\t<RECUPERACION DE ERROR %d: CORREGIR>\n",NES);
                                          
                                     }
                                     break;
                    		case 'L':
                                      if (c==';')
                                     {
                                           /*SACAMOS EL CONTENIDO DE  L y su Atributo*/
                                           POP();
                                           POP();
                                           /*/////////////////////////////////////*/
                                           /*OBTENEMOS ATRIBUTO SOBRE LA CADENA DE ENTRADA*/
                                           att=GetAttributo_ATOMO();
                                           PRINT_C(att);
                                           AVANZA();
                                           /*/////////////////////////////////////*/
                                     }
                                     else if (c==',')
                                     {
                                          /*
                                          AVANZA();
                                          POP()
	                                      t POP();
	                                      PUSH(t)
	                                      PUSH(L)
	                                      PUSH(NIL)
	                                      PUSH(t)
	                                      PUSH({AT})
	                                      PUSH(SP-2)
	                                      PUSH(a)


                                          */
                                           POP();
                                           att_tipo=(*TOPE).dato;
                                           POP();
                                           PUSH(att_tipo);
                                           PUSH('L');
                                           PUSH('-');
                                           PUSH(att_tipo);
                                            /*PUSH{AT}*/
                                                       PUSH('#');
                                                       (*TOPE).NUM_ACCION=ASIGNA_TIPO;
                                           
                                           /*FIN DE ACCION*/
                                            /*PUSH(SP-2)*/
                                                       PUSH('-');
                                                       (*TOPE).REF=GetTope_n(2);
                                           /*FIN DE PUSH (SP-2)*/
                                           PUSH('a');
                                           /*LECTURA DEL ATRIBUTO Y SU ATOMO*/
                                           att=GetAttributo_ATOMO();
                                           PRINT_C(att);
                                           AVANZA();
                                           /********************************/
                                     }
                                     else
                                     {
                                          errorS(" ,  ;");
                                          Recuperacione(RE_P,"");
                                          
                                     }
                                     break;
                    		case 'T':
                                     if (c=='c'||c=='i')
                                     {
                                                        /*
                                                        POP()
                                                        ap<--POP
                                                        *ap<-Token
                                                        AVANZA()
                                                        */

                                           POP();
                                           /* ap<--POP();
                                              *ap<--Token
                                           */
                                           AUXNP=(*TOPE);
                                           AUXANP=AUXNP.REF;
                                           att_tipo=GetAttributo_ATOMO();
                                           (*AUXANP).dato=att_tipo;
                                           (*(*AUXANP).REF).dato=att_tipo;
                                           POP();
                                           /*OPERACIONES SOBRE LA CADENA YA QUE 
                                           HEMOS OBTENIDO EL TIPO LO MOSTRAMOS*/
                                           PRINT_C(att_tipo);
                                           AVANZA();
                                     }
                                     else
                                     {
                                          errorS(" char int");
                                          /*
                                          Recuperacione(RE_I,"");
                                          AVANZA();
                                          POP();
                                          
                                          */
                                          POP();
                                          AUXNP=(*TOPE);
                                          AUXANP=AUXNP.REF;
                                          att_tipo=GetAttributo_ATOMO();
                                          (*AUXANP).dato='@';
                                          (*(*AUXANP).REF).dato='@';
                                          POP();
                                          AVANZA();
                                          printf("\t<RECUPERACION DE ERROR %d:IGNORAR>\n",NES);
                                          
                                     }
                                     break;
                    		case 'S':
                                   if (c=='f')
                                     {
                                           REEM_R("F");
                                           RETEN();
                                           
                                     }
                                     else if(c=='w')
                                     {
                                           REEM_R("W");
                                           RETEN();
     
                                     }
                                      else if(c=='d')
                                     {
                                           REEM_R("H");
                                           RETEN();
     
                                     }
                                     else if(c=='a')
                                     {
                                           REEM_R("A;");
                                           RETEN();
     
                                     }
                                      else if(c=='b')
                                     {
                                           REEM_R("I");
                                           RETEN();
     
                                     }
                                     else
                                     {
                                         errorS("for while do  if identificador");
                                         Recuperacione(RE_P,"");
                                     }
                                     break;
                    		case 'F':
                                     if(c=='f')
                                     {
                                          /*
                                           REEM_R("(A;E;A)P");
                                           Con atributos y simbolos de accion
                                                   ( 
                                                   [QCAR] 
                                                   A
                                                   ;
                                                   [PRINT WHILE(] 
                                                   E
                                                   ;
                                                   [QCAR]
                                                   [PRINT ){ ]
                                                   [SET_INC]
                                                   A
                                                   [SET_OFF_INC]
                                                   )
                                                   [QCAR]
                                                   P 
                                                   [PRINT BUFF_INC;]
                                                   *************************
                                                   REEM_R("(A;E;A)P");
                                          */
                                           POP();
                                           /***********************************/
                                            PUSH('#');
                                           (*TOPE).NUM_ACCION=PRINT_CIERRE;
                                           
                                           PUSH('#');
                                           (*TOPE).NUM_ACCION=PRINT_BUFFINC;
                                           
                                           PUSH('P');
                                           
                                           PUSH('#');
                                           (*TOPE).NUM_ACCION=QCAR;
                                           
                                            PUSH(')');
                                            
                                            PUSH('#');
                                           (*TOPE).NUM_ACCION=SET_OFFINC;
                                           
                                            PUSH('A');
                                            
                                            PUSH('#');
                                           (*TOPE).NUM_ACCION=SET_ONINC;
                                           
                                           PUSH('#');
                                           (*TOPE).NUM_ACCION=PRINT_PAR;
                                           
                                           PUSH('#');
                                           (*TOPE).NUM_ACCION=QCAR;
                                           
                                            PUSH(';');
                                            
                                            PUSH('E');
                                            
                                             PUSH('#');
                                           (*TOPE).NUM_ACCION=PRINT_WHILE;
                                           
                                            PUSH(';');
                                            
                                            PUSH('A');
                                            
                                             PUSH('#');
                                           (*TOPE).NUM_ACCION=QCAR;
                                           
                                            PUSH('(');
                                           /***********************************/
                                           
                                           att=GetAttributo_ATOMO();
                                           /*
                                           YA NO NOS INTERESA IMPRIMIR EL ATOMO
                                           PRINT_C(att);
                                           */
                                           AVANZA();
                                           
                                           /***********************************
                                           ESTA PRODUCCION ES LA RAZON DE SER DE
                                           LA NUEVA ETAPA DEL ANALIZADOR COMO UN 
                                           TRADUCTOR
                                           ************************************/
                                     }
                                     else
                                     {
                                         errorS("for");
                                         printf("\t<RECUPERACION DE ERROR %d: CORREGIR>\n",NES);
                                         
                                         /*
                                         Recuperacione(RE_C,"(A;E;A)P");
                                         case RE_C:
                                              RETEN();
                                              REEM_R(buf);
                                              
                                         break;
                                         */
                                         POP();
                                         
                                            PUSH('#');
                                           (*TOPE).NUM_ACCION=PRINT_CIERRE;
                                           
                                           PUSH('#');
                                           (*TOPE).NUM_ACCION=PRINT_BUFFINC;
                                           
                                           PUSH('P');
                                           
                                           PUSH('#');
                                           (*TOPE).NUM_ACCION=QCAR;
                                           
                                            PUSH(')');
                                            
                                            PUSH('#');
                                           (*TOPE).NUM_ACCION=SET_OFFINC;
                                           
                                            PUSH('A');
                                            
                                            PUSH('#');
                                           (*TOPE).NUM_ACCION=SET_ONINC;
                                           
                                           PUSH('#');
                                           (*TOPE).NUM_ACCION=PRINT_PAR;
                                           
                                           PUSH('#');
                                           (*TOPE).NUM_ACCION=QCAR;
                                           
                                            PUSH(';');
                                            
                                            PUSH('E');
                                            
                                             PUSH('#');
                                           (*TOPE).NUM_ACCION=PRINT_WHILE;
                                           
                                            PUSH(';');
                                            
                                            PUSH('A');
                                            
                                             PUSH('#');
                                           (*TOPE).NUM_ACCION=QCAR;
                                           
                                            PUSH('(');
                                           
                                           RETEN();
                                          
                                         
                                     }
                                     break;
                    		case 'P':
                                     if(c==';')
                                     {
                                           POP();
                                           att=GetAttributo_ATOMO();
                                           PRINT_C(att);
                                           AVANZA();
     
                                     }
                                     else if(c=='f'||c=='w'||c=='d'||c=='a'||c=='b')
                                     {
                                          REEM_R("S");
                                          RETEN();
                                     } 
                                     else if(c=='{')
                                     {
                                          REEM_R("B}");
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else
                                     {
                                         errorS("  for while do if identificador ; {");
                                         Recuperacione(RE_P,"");
                                     }
                                     break;
                    		case 'B':
                                     if(c=='}')
                                     {
                                           POP();
                                           RETEN();
                                     }
                                     else if(c=='f'||c=='w'||c=='d'||c=='a'||c=='b')
                                     {
                                          REEM_R("SB");
                                          RETEN();
                                     } 
                                     else if(c=='{')
                                     {
                                          REEM_R("B}B");
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else
                                     {
                                         errorS("{ for while do if identificador }");
                                         Recuperacione(RE_P,"");
                                     }
                                     break;
                    		case 'W':
                                     if(c=='w')
                                     {
                                          REEM_R("(E)P");
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else
                                     {
                                         errorS("while");
                                         Recuperacione(RE_C,"(E)P");
                                     }
                                     break;
                    		case 'I':
                                      if(c=='b')
                                     {
                                          REEM_R("(E)P");
                                           att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else
                                     {
                                         errorS("if");
                                         Recuperacione(RE_C,"(E)P");
                                     }
                                     break;
                    		case 'H':
                                     if(c=='d')
                                     {
                                          REEM_R("Pw(E);");
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else
                                     {
                                         errorS("do");
                                         Recuperacione(RE_C,"Pw(E);");
                                     }
                                     break;
                    		case 'A':
                                     if(c=='a')
                                     {
                                     /*
                                     AVANZA()
		                             pos=GetPOS();
		                             RD(p)
		                             Reem(OE)R
		                             */
                                          att_pos=GetAttributoPos();
                                          RD(att_pos);
                                          PRINT_ID(att_pos);
                                          AVANZA();
                                          REEM_R("OE");
                                          
                                        
                                     }
                                     else
                                     {
                                         errorS("identificador");
                                         Recuperacione(RE_C,"OE");
                      
                                     }
                                     break;
                    		case 'O':
                                     if(c=='='||c=='+'||c=='-'||c=='*'||c=='/')
                                     {
                                          POP();
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else
                                     {
                                         errorS("asignacion");
                                         Recuperacione(RE_I,"");
                                         
                                     }
                                     break;
                    		case 'K':
                                     if(c=='a'||c=='o'||c=='x'||c=='h')
                                     {
                                          POP();
                                          if(c=='a')
                                          {
                                                    att_pos=GetAttributoPos();
                                                    RD(att_pos);
                                                    PRINT_ID(att_pos);
                                          }
                                          else
                                          {
                                                    att=GetAttributo_ATOMO();
                                                    PRINT_C(att);
                                              
                                          }
                                          AVANZA();
                                     }
                                     else
                                     {
                                         errorS("identifcador o entero");
                                         Recuperacione(RE_I,"");
                                         
                                     }
                                     break;
                    		case 'E':
                                     if(c=='~'||c=='('||c=='a'||c=='o'||c=='x'||c=='h'||c=='!')
                                     {
                                          REEM_R("CV");
                                          RETEN();
                                     }
                                     else
                                     {
                                         errorS("~ (  identificador entero !");
                                         Recuperacione(RE_C,"V");
                                         
                                     }
                                     break;
                    		case 'V':
                                     if(c=='>'||c=='g'||c=='<'||c=='l'||c=='e'||c=='n')
                                     {
                                          REEM_R("XCV");
                                          RETEN();
                                     }
                                     else if(c==';'||c==')')
                                     {
                                          POP();
                                          RETEN();
                                     }
                                     else
                                     {
                                         errorS(" > => < <= = !=");
                                         Recuperacione(RE_P,"");
                                         
                                     }
                                     break;
                    		case 'C':
                                     if(c=='~'||c=='('||c=='a'||c=='o'||c=='x'||c=='h'||c=='!')
                                     {
                                          REEM_R("JQ");
                                          RETEN();
                                     }
                                     else
                                     {
                                         errorS("~ (  identificador entero !");
                                         Recuperacione(RE_C,"Q");
                                         
                                     }
                                     break;
                    		case 'Q':
                                    if(c=='>'||c=='g'||c=='<'||c=='l'||c=='e'||c=='n'||c==';'||c==')')
                                     {
                                          POP();
                                          RETEN();
                                     }
                                     else if(c=='y'||c=='r'||c=='&'||c=='|'||c=='^'||c=='s'||c=='z')
                                     {
                                          REEM_R("UJQ");
                                          RETEN();
                                     }
                                     else
                                     {
                                         errorS("> => < <= = !=  ; ) && || & | ^  >> <<"); 
                                          Recuperacione(RE_P,"");
                                         
                                     }
                                     break;
                    		case 'J':
                                     if(c=='~')
                                     {
                                          REEM_R("K");
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else if(c=='(')
                                     {
                                          REEM_R("E)");
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else if(c=='a'||c=='o'||c=='x'||c=='h')
                                     {
                                          REEM_R("K");
                                          RETEN();
                                     }
                                     else if(c=='!')
                                     {
                                          REEM_R("N");
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else
                                     {
                                         errorS("~ ( identificador entero !");
                                          Recuperacione(RE_P,"");

                                     }
                                     break;
                    		case 'N':
                                     if(c=='a'||c=='o'||c=='x'||c=='h')
                                     {
                                          REEM_R("K");
                                          RETEN();
                                     }
                                     else if(c=='(')
                                     {
                                          REEM_R("E)");
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else
                                     {
                                         errorS("entero identificador");
                                         Recuperacione(RE_P,"");
                                         
                                     }
                                     break;
                    		case 'U':
                                     if(c=='y'||c=='r'||c=='&'||c=='|'||c=='^'||c=='s'||c=='z')
                                     {
                                          POP();
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else
                                     {
                                         errorS(" && || & | ^  >> <<"); 
                                          Recuperacione(RE_I,"");
                                        
                                     }
                                     break;
                    		case 'X':
                                     if(c=='>'||c=='g'||c=='<'||c=='l'||c=='e'||c=='n')
                                     {
                                          POP();
                                          att=GetAttributo_ATOMO();
                                          PRINT_C(att);
                                          AVANZA();
                                     }
                                     else
                                     {
                                        errorS(" > => < <= = !=");
                                        Recuperacione(RE_I,"");
                                        
                                     }
                                     break;
                            case '}':
                                    if (c=='}')
                                    {
                                       POP();
                                       att=GetAttributo_ATOMO();
                                       PRINT_C(att);
                                       AVANZA();
                                    }
                                    else
                                    {
                                        errorS(" }"); 
                                        Recuperacione(RE_I,"");
                                        
                                    }
                                    break;
                            case 'a':
				    
                                    if (c=='a')
                                    {
					   
                                           POP();
                                           /* ap<--POP();
                                              *ap<--Token
                                           */
                                           AUXNP=(*TOPE);
                                           AUXANP=AUXNP.REF;
                                           att_pos=GetAttributoPos();
                                           (*AUXANP).pos=att_pos;
                                           //printf("\nPOS:%d",(*AUXANP).pos);
                                           POP();
                                           PRINT_ID(att_pos);
                                           AVANZA();
                                      
                                    }
                                    else
                                    {
                                        errorS(" identificador");
                                        Recuperacione(RE_I,""); 
                                    }
                                    break;
                            case ';':
                                    if (c==';')
                                    {
                                       POP();
                                       att=GetAttributo_ATOMO();
                                       PRINT_C(att);
                                       AVANZA();
                                    }
                                    else
                                    {
                                        errorS(" ;"); 
                                        Recuperacione(RE_I,"");
                                    }
                                    break;
                            case '(':
                                    if (c=='(')
                                    {
                                       POP();
                                       att=GetAttributo_ATOMO();
                                       PRINT_C(att);
                                       AVANZA();
                                    }
                                    else
                                    {
                                        errorS(" ("); 
                                        Recuperacione(RE_I,"");
                                    }
                                    break;
                            case ')':
                                    if (c==')')
                                    {
                                       POP();
                                        att=GetAttributo_ATOMO();
                                       PRINT_C(att);
                                       AVANZA();
                                    }
                                    else
                                    {
                                        errorS(" )"); 
                                        Recuperacione(RE_I,"");
                                    
                                    }
                                    break;
                            case 'w':
                                    if (c=='w')
                                    {
                                       POP();
                                        att=GetAttributo_ATOMO();
                                       PRINT_C(att);
                                       AVANZA();
                                    }
                                    else
                                    {
                                        errorS(" while"); 
                                        Recuperacione(RE_I,"");
                                      
                                    }
                                    break;
                            case '#':
                                     
                                     ACCION((*TOPE).NUM_ACCION);
                                     break;
                            case '\0':
                                        EP=1;
                                        errorS("FIN DE ARCHIVO"); 
                                        /*VER MANEJO DE ERRORES*/
                                    break;
     } 
         
           /*USADOS PARA PRUEBAS:*/
           /*
           printf("\n::::::::::::\nTOPE dato:%c",GetTope());
           printf("\nCARACTER w actual: %c",eas[j]);
           printf("\n::::::::::::\n");
           PINTA_PILA();
           
           getchar();
	   */
           

	  
	   
		
     
}   
    if(GetTope()=='G'){ errorS("}");}                   
    else if(c=='\0' && GetTope()=='\0') {A=1;}
    PINTA_TS();
    printf("\n:::Resultado del Analisis Sintactico:\n");
    if(A==1 && EP==0 && ERROR_S==0)
    {
            printf("\n\t1)Cadena Aceptada");
            fprintf(LOG_ERROR,"\t No hubo errores en el analisis sintactico.CADENA ACEPTADA"); 
            printf("\n\n\t::Cadena traducida:\n\t");
	    fprintf(LOG_ERROR,"\n\n\t::Cadena traducida:\n\t");
	    printf("%s\n",cad_trad);
	    fprintf(LOG_ERROR,"%s\n",cad_trad);
                
    }
    else
    {
            F_ERROR(2);
            
    }
};
char* FormatoCAD_ATM()
{ 	
      int i,j,n;
      n=strlen(eas);
      palabraini=(char*)malloc((n+1)* sizeof(char));
      //palabraini=(char*)realloc(palabraini,(n+1)*sizeof(char));
      strcpy(palabraini,eas);
      for(i=0;i<n;i++)
      {
                      while(palabraini[i]=='_')
                      {
                                     for(j=i;j<n;j++)
                                    {
                                                     palabraini[j]=palabraini[j+1];
                                    }
                      }
      }
      return palabraini;
};
int AnalisisLexico(){     
        /*Inicializamos tablas*/
        InicializaAFD();
        InicializarTablas();
        /*Analisis Lexico*/
        fprintf(LOG_ERROR,":::::LOG DE EVENTOS::::::\n1)Analisis lexico\n");
        c=fgetc(AIN);
        ALex();
	    verificarNC();
        /*Cerramos archivos que ya no se usaran*/
        fclose(AIN);
     
        return ERROR_G==0; 
        
};
int AnalisisSintactico(){     
        /*No hubo errores en el analisis lexico*/
         LINEA=1;
         CreaPila();
	 
         InicializaCadena();
         
         
         fprintf(LOG_ERROR,"\n2)Analisis Sintactico");
         fprintf(LOG_ERROR,"\n\t:Cadena de atomos generada :%s\n\n",FormatoCAD_ATM());
         printf("\n\n:::La cadena de atomos es:\n\n%s\n\n",FormatoCAD_ATM());
         printf("\n:::Eventos Analisis Sintactico: \n\n");
         fprintf(LOG_ERROR,"\t::Eventos Analisis Sintactico:\n");
         
         PUSH('M');
	 
         ASin();
         
 	 /*LIBERAMOS BUFFERS Y MEMORIA DINAMICA*/
         palabraini=NULL;
	     free(palabraini);
         
         eas =NULL;
         free(eas); 
         
	     TS_TEMP=NULL;
	     free(TS_TEMP);
	     
	     cad_trad=NULL;
	     free(cad_trad);
	     
         return ERROR_G==0;  
};

int main (int argc , char **argv )
{   

    /* ABRIMOS ARCHIVOS*/
    AIN=fopen(argv[1],"r");
    //AIN=fopen("B.txt","r");
    if (AIN==NULL)
       printf("No existe el Archivo especificado\n");
    else
    {
        TTOKENS=fopen("TOKENS.txt","w");
        TS=fopen("TS.txt","w+r");
        LOG_ERROR=fopen("LOG_EVENTOS.txt","w+r");
       
        if (AnalisisLexico())
        {
          if(AnalisisSintactico())
          {
            printf("\n\n:::FIN\n");
          }
        }
        /*Cerramos archivos*/
        fclose(TS);
	    /* TIRA UN ERROR DE LINKED LISTfclose(TTOKENS);*/
        fclose(LOG_ERROR);
 	
    }
    
    return 0;
}
