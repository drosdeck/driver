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

static enum power_supply_property test_battery_properties[] = {
    POWER_SUPPLY_PROP_STATUS,
    POWER_SUPPLY_PROP_CHARGE_TYPE,
    POWER_SUPPLY_PROP_HEALTH,
    POWER_SUPPLY_PROP_PRESENT,
    POWER_SUPPLY_PROP_TECHNOLOGY,
    POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN,
    POWER_SUPPLY_PROP_CHARGE_FULL,
    POWER_SUPPLY_PROP_CHARGE_NOW,
    POWER_SUPPLY_PROP_CAPACITY,
    POWER_SUPPLY_PROP_CAPACITY_LEVEL,
    POWER_SUPPLY_PROP_TIME_TO_EMPTY_AVG,
    POWER_SUPPLY_PROP_TIME_TO_FULL_NOW,
    POWER_SUPPLY_PROP_MODEL_NAME,
    POWER_SUPPLY_PROP_MANUFACTURER,
    POWER_SUPPLY_PROP_SERIAL_NUMBER,
    POWER_SUPPLY_PROP_TEMP,
    POWER_SUPPLY_PROP_VOLTAGE_NOW,
};



static char *fake_ac_supplies[] = {
    "BAT0",
};

static const struct power_supply_desc test_power_desc = {
	.name = "test_bat",
	.type = POWER_SUPPLY_TYPE_BATTERY,
	.properties =  test_battery_properties,
	.num_properties = ARRAY_SIZE(test_battery_properties)

	};


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
 
