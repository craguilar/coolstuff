/*
 Tenemos la definicion de las llamaas al sistema
 en este archivo que nos redirrigira, asi:
 unistd.h
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
 #include <asm/system.h>
 
MODULE_LICENSE("GPL");
 
#define START_MEM   0xc0000000
#define END_MEM     0xd0000000
 
unsigned long *syscall_table; 
 
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
 
static int init(void) {
 
    printk("\nModule starting...\n");
 
    syscall_table = (unsigned long *) find();
 
    if ( syscall_table != NULL ) {
     
        printk("Syscall table found at %x\n", (unsigned ) syscall_table);
     
    } else {
     
        printk("Syscall table not found!\n");
         
    }   
 
    return 0;
}
 
static void exit(void) {
 
    printk("Module ending\n");
 
    return;
}
 
module_init(init);
module_exit(exit);
