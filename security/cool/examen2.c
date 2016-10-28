#include <stdio.h>

char input[] = "BBBECLAFRI2\1\11O\1S1LL081";

int main(void){
	int i,c;
	for(i=2;(c=input[i]) != '\0';i++){
		switch(c){
			case 'a' : putchar('i');
				printf("a\n");	 
				continue;
					
			case '1' : break;
			case  1  : while ((c=input[++i]) != '\1' && c!='\0' );
			case  9  : putchar('O');
			case 'F' : case 'L': case '2': continue;
			default  : putchar(c); continue;
	
		
		}
		putchar(' ');
	}
	printf("\n");
}
