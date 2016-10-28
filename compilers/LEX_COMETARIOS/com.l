%{
 //Programa que identifica comentarios en C 1 er examen 
 FILE *ArchSal;
%}
inicio \/\*
fin \*\/
cc [^\*]
ce \*[^\/]
com  {inicio}({cc}|{ce})*{fin}
%%

{com} {fprintf(ArchSal,"<%s> es un comentario.\n",yytext);}

%%
main(int argc, char *argv[])
{
 yyin=fopen(argv[1],"r");
 ArchSal=fopen("salida.txt","w");
 yylex();
 fclose(ArchSal);
}

