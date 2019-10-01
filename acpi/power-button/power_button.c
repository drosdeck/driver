#include <linux/module.h>     /* Needed by all modules */ 
#include <linux/kernel.h>     /* Needed for KERN_INFO */ 
#include <linux/init.h>       /* Needed for the macros */ 
#include <linux/types.h>
//#include <acpi/acpi_bus.h>
//#include <acpi/acpi_drivers.h>  
#include <linux/acpi.h>



MODULE_LICENSE("GPL"); 
  
MODULE_AUTHOR("Edson Juliano Drosdeck"); 
  
MODULE_DESCRIPTION("Power button action"); 


MODULE_VERSION("0.1"); 
 //PNP0C0C
 
static const struct acpi_device_id bt_device_ids[] = {
	{ "PNP0C0C", 0},
        { "", 0},
    };
MODULE_DEVICE_TABLE(acpi, bt_device_ids);

static int  acpi_power_button_add(struct acpi_device *device)
{

	return 0;
}

static int acpi_power_button_remove(struct acpi_device *device)
 {
	return 0;
}

static struct acpi_driver power_button_driver = {
        .name =         "Power button",
        .class =        "Policorp",
        .ids =          bt_device_ids,
        .ops =          {
                                .add =     acpi_power_button_add,
                                .remove =  acpi_power_button_remove,
                        },
	.owner =	THIS_MODULE,
    };

static int __init power_button_start(void) 
{ 
    int result = 0;

        result = acpi_bus_register_driver(&power_button_driver);
        if (result < 0) {
                ACPI_DEBUG_PRINT((ACPI_DB_ERROR,
                                  "Error registering driver\n"));
                return -ENODEV;
        }

        return 0;
	
	
	printk(KERN_INFO "ola amigo\n"); 
    return 0; 
} 
  
static void __exit power_button_end(void) 
{ 
    acpi_bus_unregister_driver(&power_button_driver);
	printk(KERN_INFO "Adios amigo \n"); 
} 
  
module_init(power_button_start); 
module_exit(power_button_end); 
