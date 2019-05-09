#include <linux/module.h>	 
#include <linux/kernel.h>	 
#include <linux/init.h>	  
#include <linux/acpi.h>
#include <linux/acpi_bus.h>
#include <linux/rfkill.h>
#include <linux/types.h>
MODULE_LICENSE("GPL"); 

MODULE_AUTHOR("Edson Juliano Drosdeck"); 

MODULE_DESCRIPTION("bluetooth rfkill to j14kr11"); 

MODULE_VERSION("1.0"); 






static int __init oemwmi_start(void) 
{
	int status;
        int err;

	return 0; 
} 

static void __exit oemwmi_end(void) 
{ 
	
} 

module_init(oemwmi_start); 
module_exit(oemwmi_end); 

