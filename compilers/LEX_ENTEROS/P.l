%{
 FILE *ArchSal;
%}
digO [1-7]
digD [1-9]
digH {digD}|[a-f]|[A-F]
decimal {digD}({digD}|0)*|0
Octal 0({digO}|0)+
Hexadecimal 0[xX]({digH}|0)+
%%
{decimal} {fprintf(ArchSal,"%s es decimal\n",yytext);}
{Octal} {fprintf(ArchSal,"%s es octal\n",yytext);}
{Hexadecimal} { fprintf(ArchSal,"%s es hexadecimal\n",yytext);}
%%
main(int argc, char *argv[])
{
 yyin=fopen(argv[1],"r");
 ArchSal=fopen("salida.txt","w");
 yylex();
 fclose(ArchSal);
}

