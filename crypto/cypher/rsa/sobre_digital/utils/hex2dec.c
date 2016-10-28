/* 
   Compilar con:
   gcc hex2dec.c -lssl -o hex2dec
*/ 

#include <stdio.h>
#include <openssl/bn.h>

int main (int argc, char **argv)
{
  BIGNUM *n = BN_new();
  if (argc!=2)
    {
      printf("%s <hex>\n", argv[0]);
      return 0;
    }
  if (!BN_hex2bn(&n, argv[1]))
    {
      printf("error: BN_hex2bn()");
      return 0;
    }
  printf("%s\n", BN_bn2dec(n));
  BN_free(n);
}
