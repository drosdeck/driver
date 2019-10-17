#include <linux/module.h>	 /* Needed by all modules */ 
#include <linux/kernel.h>	 /* Needed for KERN_INFO */ 
#include <linux/init.h>	 /* Needed for the macros */ 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Edson Juliano drosdeck"); 
MODULE_DESCRIPTION("handle to acpi LID switch"); 
MODULE_VERSION("0.1"); 

static int __init xxx_wmi_start(void) 
{ 
	printk(KERN_INFO "Hello world\n"); 
	return 0; 
} 

static void __exit xxx_wmi_end(void) 
{ 
	printk(KERN_INFO "Goodbye Mr.\n"); 
} 

module_init(xxx_wmi_start); 
module_exit(xxx_wmi_end); 

