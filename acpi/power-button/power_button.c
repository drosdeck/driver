#include <linux/module.h>     /* Needed by all modules */ 
#include <linux/kernel.h>     /* Needed for KERN_INFO */ 
#include <linux/init.h>       /* Needed for the macros */ 
  
MODULE_LICENSE("GPL"); 
  
MODULE_AUTHOR("Edson Juliano Drosdeck"); 
  
MODULE_DESCRIPTION("Power button action"); 
  
MODULE_VERSION("0.1"); 
  
static int __init power_button_start(void) 
{ 
    printk(KERN_INFO "ola amigo\n"); 
    return 0; 
} 
  
static void __exit power_button_end(void) 
{ 
    printk(KERN_INFO "Adios amigo \n"); 
} 
  
module_init(power_button_start); 
module_exit(power_button_end); 
