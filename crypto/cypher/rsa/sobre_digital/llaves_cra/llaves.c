#include <stdio.h>
#include <gmp.h>

/*
Compilar
gcc -o llaves llaves.c -lgmp
*/

char *cifra(char *m,mpz_t exp, mpz_t mod){
    mpz_t mens;
    mpz_init(mens);
    mpz_set_str(mens,m,10);
    printf("\n%s\n",mpz_get_str(NULL,10,mens));
    mpz_powm(mens,mens,exp,mod);
    return mpz_get_str(NULL,10,mens);
}

int main(int argv, char* argc[]){
    mpz_t n,p,q,e,d,phi,uno;
    unsigned long int l;
    int test;
    char *p_str,*q_str;
    srandom(time(NULL));
    mpz_init(n);
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(d);
    mpz_init(phi);
    mpz_init(uno);
    l=random()%100;
    //sscanf(argc[1],"%d",&l);
    l=133;
    //l=;
    if(l<0){l=50;}
    mpz_ui_pow_ui(p,l+random()%30,l+random()%30);
    mpz_nextprime(p,p);
    l=random()%100;

    mpz_ui_pow_ui(q,l+random()%30,l+random()%30);
    mpz_nextprime(q,q);
    
    p_str=mpz_get_str(NULL,10,p);
    q_str=mpz_get_str(NULL,10,q);

    //printf("p: \n%s\n",p_str);
    //printf("q: \n%s\n",q_str);

    mpz_sub_ui(e,p,1);
    mpz_sub_ui(d,q,1);

    mpz_mul(n,p,q);
    mpz_mul(phi,e,d);

    
    mpz_set_ui(e,65537);

    mpz_invert(d,e,phi);
    q_str=mpz_get_str(NULL,10,n);

    //printf("n: \n%s\n",q_str);

    //printf("e: \n%s\n",mpz_get_str(NULL,10,e));
    //printf("d: \n%s\n",mpz_get_str(NULL,10,d));
    
    printf("Llave publica: (%s,%s)\n\n",mpz_get_str(NULL,10,e),mpz_get_str(NULL,10,n));
    printf("Llave privada: (%s,%s)\n\n",mpz_get_str(NULL,10,d),mpz_get_str(NULL,10,n));

    printf("Otros valores:\np: %s\nq: %s\n\n",mpz_get_str(NULL,10,p),mpz_get_str(NULL,10,q));

}
