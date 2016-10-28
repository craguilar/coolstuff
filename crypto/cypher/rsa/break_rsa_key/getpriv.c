ompilar con:
   gcc obtener_privada.c -lssl -o obtener_privada
*/

#include <stdio.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>

int main (int argc, char **argv) 
{
   RSA *keypair = RSA_new();

   BN_CTX *ctx = BN_CTX_new();
   BN_CTX_start(ctx);

   BIGNUM *n = BN_new();
   BIGNUM *d = BN_new();
   BIGNUM *e = BN_new();
   BIGNUM *p = BN_new();
   BIGNUM *q = BN_new();
   BIGNUM *dmp1 = BN_new();
   BIGNUM *dmq1 = BN_new();
   BIGNUM *iqmp = BN_new();

   BIGNUM *r0 = BN_CTX_get(ctx);
   BIGNUM *r1 = BN_CTX_get(ctx);
   BIGNUM *r2 = BN_CTX_get(ctx);
   BIGNUM *r3 = BN_CTX_get(ctx);


   if (argc!=4) 
   {
      printf ("Uso: %s [p] [q] [exp] \n", argv[0]);
      return 0;
   }

   BN_dec2bn(&p, argv[1]);
   BN_dec2bn(&q, argv[2]);
   BN_dec2bn(&e, argv[3]);

   if(BN_cmp(p, q)<0)
   {
      BIGNUM *tmp = p;
      p = q;
      q = tmp;
   }

   // Calculamos n
   BN_mul(n, p, q, ctx);

   // Calculamos d
   BN_sub(r1, p, BN_value_one()); // p-1 
   BN_sub(r2, q, BN_value_one()); // q-1/
   BN_mul(r0, r1, r2, ctx);       // (p-1)(q-1)
   BN_mod_inverse(d, e, r0, ctx); // d 

   // Calculamos d mod (p-1)
   BN_mod(dmp1, d, r1, ctx); 

   // Calculamos d mod (q-1)
   BN_mod(dmq1, d, r2, ctx); 

   // Calculamos el inverso de q mod p 
   BN_mod_inverse(iqmp, q, p, ctx);

   // Claves RSA
   keypair->n = n;
   keypair->d = d;
   keypair->e = e;
   keypair->p = p;
   keypair->q = q;
   keypair->dmq1 = dmq1;
   keypair->dmp1 = dmp1;
   keypair->iqmp = iqmp;
   
   PEM_write_RSAPrivateKey(stdout, keypair, NULL, NULL, 0, NULL, NULL); 

   BN_CTX_end(ctx);
   BN_CTX_free(ctx);
   RSA_free(keypair);
   return 0;
}
