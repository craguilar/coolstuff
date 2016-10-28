#include <stdio.h>


float suma(int a,int b);
float (*f)(int,int);


int main(){
	printf("Ejemplo de punteros a funciones\n");
	printf("El valor de suma es=%f\n",suma(5,6));	
        f=&suma;
	// La linea anterior es lo mismo que f=suma al menos en funciones y arreglos
	// int *p=&x obtenemos la direccion de x
	// *p=5 decimos que ahora x vales 5  o se escribio donde se contenia
	// &p direccion de p , es decir donde esta
	printf("El apuntador a funcion suma es=%f\n",f(7,14));
} 

float suma(int a, int b){
	return a+=b;
}
