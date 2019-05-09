#include <linux/module.h>	 
#include <linux/kernel.h>	 
#include <linux/init.h>	  
#include <linux/acpi.h>
#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>
#include <linux/rfkill.h>
#include <linux/types.h>
MODULE_LICENSE("GPL"); 

MODULE_AUTHOR("Edson Juliano Drosdeck"); 

MODULE_DESCRIPTION("bluetooth rfkill to j14kr11"); 

MODULE_VERSION("1.0"); 

static const struct acpi_device_id  bt_device_ids[] = {
	{ "INT3420",0},
        {"",0},
};

static int positivo_bt_enable(acpi_handle handle)
{
	acpi_status result;
	acpi_integer status;


return 0;
}

static int positivo_bt_rfkill_add(struct acpi_device *device)
{
	return positivo_bt_enable(device->handle);

}

static int positivo_bt_rfkill_remove(struct acpi_device *device)
{
	return 0;

}


static struct acpi_driver positivo_bt_driver = {
	.name = "Positivo BT",
	.class = "Positivo",
	.ids = bt_device_ids,
	.ops  = {
		.add = positivo_bt_rfkill_add,
                .remove = positivo_bt_rfkill_remove,
	        },
	.owner = THIS_MODULE,
};

static int __init positivo_bt_rfkill_start(void) 
{
	int result = 0;
        int err;

	return 0; 
} 

static void __exit positivo_bt_rfkill_end(void) 
{ 
	
} 

module_init(positivo_bt_rfkill_start); 
module_exit(positivo_bt_rfkill_end); 

