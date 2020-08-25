#include <linux/module.h>     /* Needed by all modules */ 
#include <linux/kernel.h>     /* Needed for KERN_INFO */ 
#include <linux/init.h>       /* Needed for the macros */ 
#include <linux/types.h>
#include <linux/acpi.h>
#include <linux/power_supply.h>


static const struct acpi_device_id battery_device_ids[] = {
        { "PNP0C0A", 0},
        { "", 0},
    };

MODULE_DEVICE_TABLE(acpi, battery_device_ids);

static int acpi_battery_add(struct acpi_device *device)
    {
	//acpi_device_bid(device) retorna BAT0
	printk("Valor do bateria %s\n", acpi_device_bid(device));
	return 0;
    }

static void acpi_battery_notify(struct acpi_device *device, u32 event)
{

	printk("entrou na notificacao\n");


}
static struct acpi_driver my_battery_driver = {
        .name =         "My battery",
        .class =        "battery",
        .ids =          battery_device_ids,
	.flags =        ACPI_DRIVER_ALL_NOTIFY_EVENTS,
        .ops =          {
                                .add =          acpi_battery_add,
                               // .remove =       toshiba_bt_rfkill_remove,
			       .notify = acpi_battery_notify,
                        },
	.owner =	THIS_MODULE,
    };



static int __init my_bat_start(void) 
{ 
    int result = 0;

        result = acpi_bus_register_driver(&my_battery_driver);
        if (result < 0) {
		printk("Error Registerind driver\n");
                ACPI_DEBUG_PRINT((ACPI_DB_ERROR,
                                  "Error registering driver\n"));
                return -ENODEV;
        }

        return 0;
	
} 
  
static void __exit my_bat_end(void) 
{ 
    printk(KERN_INFO "Goodbye Mr.\n");
   acpi_bus_unregister_driver(&my_battery_driver); 
} 
  
module_init(my_bat_start); 
module_exit(my_bat_end); 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Edson Juliano Drosdeck"); 
MODULE_DESCRIPTION("simple battery driver"); 
MODULE_VERSION("0.1"); 
 
