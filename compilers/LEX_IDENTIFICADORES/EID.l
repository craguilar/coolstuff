%{
 #include<string.h> 
 FILE *ArchSal;
%}
let [a-z]|[A-Z]
dig [0-9]
id (_|{let})({dig}|{let}|_)*

%%
{id} {
	if( strlen(yytext)>31)
		yytext[31]= '\0';
	fprintf(ArchSal,"<%s> es el  Identificador que se meteria o buscaria en la tabla de simbolos.\n",yytext);
     }
%%
main(int argc, char *argv[])
{
 yyin=fopen(argv[1],"r");
 ArchSal=fopen("TS.txt","w");
 yylex();
 fclose(ArchSal);
}

