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

	result = acpi_evaluate_integer(handle, "BTST",NULL,&status);

	if(ACPI_FAILURE(result))
		return -EINVAL;
	
	if (!(status & (1 << 0)))
		return -EBUSY;

	if (!(status & (1 << 6)))
		result = acpi_evaluate_object(handle, "AUSB",NULL,NULL);
	
	if (ACPI_FAILURE(result)){
		printk(KERN_ERR "Failled to reattach Positivo Bluetooth device\n");
		return -ENODEV;
	}

	if (!(status & (1 << 7)))
		result = acpi_evaluate_object(handle, "BTPO",NULL,NULL);

	if (ACPI_FAILURE(result)){
		printk(KERN_ERR "Failled to power on Positivo Bluetooth device\n");
		return -ENODEV;
	}

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
       
         result = acpi_bus_register_driver(&positivo_bt_driver);
 	if (result < 0 ){
		printk("Erro no registro");
	}		
	return 0; 
} 

static void __exit positivo_bt_rfkill_end(void) 
{ 
	
} 

module_init(positivo_bt_rfkill_start); 
module_exit(positivo_bt_rfkill_end); 

