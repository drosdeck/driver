#include <linux/module.h>	 
#include <linux/kernel.h>	 

MODULE_LICENSE("GPL"); 

MODULE_AUTHOR("Edson Juliano Drosdeck"); 

MODULE_DESCRIPTION("Keyboard driver"); 

MODULE_VERSION("0.1"); 



static int __init keyboard_drv_start(void) 
{
	return 0; 
} 

static void __exit keyboard_drv_end(void) 
{ 
	
} 

module_init(keyboard_drv_start); 
module_exit(keyboard_drv_end); 

