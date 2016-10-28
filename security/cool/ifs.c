#include <stdio.h>

#define PR(k, value) printf(#value" = %"#k"\t", (value))
#define NL putchar('\n')

#define PRINT1(f,x1) PR(f,x1), NL
#define PRINT2(f,x1,x2) PR(f,x1), PRINT1(f,x2)
#define PRINT3(f,x1,x2,x3) PR(f,x1), PRINT2(f,x2,x3)
#define PRINT4(f,x1,x2,x3,x4) PR(f,x1), PRINT3(f,x2,x3,x4)

int main()
{
    int x, y = 1, z;

    if( y!=0 ) x = 5;
    PRINT1(d,x);

    if(y == 0) x = 3;
    else x = 5;
    PRINT1(d,x);

    x = 1;
    if(y < 0) if(y > 0) x = 3;
    else x = 5;
    PRINT1(d,x);

    if(z = y < 0) x=3;
    else if(y == 0) x=5;
    else x=7;
    PRINT2(d,x,z);
    
    // Tarea . Del programa que habiamos analizado 
    // hasta aqui se habia revisado la salida en clase 
    // teniamos que z=0,x=7 y y=1
    
    //  La comparacion se realiza primero , por lo cual 
    //  y==0  Falso
    //  Entonces se asigna z=0
    //  Saltamos hasta x=3
    //  Se imprimiria
    //  x=3  z=0
    if(z=(y == 0) ) x=5; x=3;
    PRINT2(d,x,z);
    // Asignamos a x y z el valor de y que es 1 o en este caso C 
    // Lo interpreta como True
    // Por lo cual x=3 z=1 y=1
    // Se imprimiria
    // x=3  z=1
    if(x=z=y); x=3;
    PRINT2(d,x,z);
    // Final
    return 0;
    
}
