#include <linux/module.h>	 
#include <linux/kernel.h>	 
#include <linux/init.h>	  
#include <linux/acpi.h>
#include <linux/string.h>
#include <linux/rfkill.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL"); 

MODULE_AUTHOR("Edson Juliano Drosdeck"); 

MODULE_DESCRIPTION("Send wmi event key to acpi_listen"); 

MODULE_VERSION("2.0"); 



static char *WMI_EVENT_GUID = "ABBC0F72-8EA1-11D1-00A0-C90629100000";
module_param(WMI_EVENT_GUID,charp,0644);
MODULE_PARM_DESC(WMI_EVENT_GUID,"Wmi event guid");

static struct rfkill *wireless_rfkill;


// platform probe

static int positivo_platform_probe(struct platform_device *device)
{



return 0;
}

static struct platform_driver positivo_platform_driver = {
	.driver = {
		.name = "positivo-wmi",
	},
        .probe = positivo_platform_probe,
};

static struct platform_device *positivo_platform_device;
//static struct platform_driver xo1_rfkill_driver = {
//	.driver = {
//		.name = "xo1-rfkill",
//	},
//	.probe		= xo1_rfkill_probe,
//	.remove		= xo1_rfkill_remove,
//};


static void wmi_hotkey_notify(u32 value, void *context)
    {
	struct acpi_buffer response = { ACPI_ALLOCATE_BUFFER, NULL };
	union acpi_object *obj;
	acpi_status status;
	int code;

	status = wmi_get_event_data(value, &response);
	if (status != AE_OK) {
	    pr_err("bad event status 0x%x\n", status);
	    return;
	}

	obj = (union acpi_object *)response.pointer;

 	if (obj && obj->type == ACPI_TYPE_INTEGER) {
		code = obj->integer.value;
               acpi_bus_generate_netlink_event("Wmi", "oem-wmi",code,ACPI_TYPE_INTEGER);
 	}

	kfree(obj);
    }



static int __init oemwmi_start(void) 
{
	int status;
        int err;

        if (!wmi_has_guid(WMI_EVENT_GUID)) {
	    printk("No known WMI GUID found\n");
	    return -ENODEV;
	}

	status = wmi_install_notify_handler(WMI_EVENT_GUID,
					wmi_hotkey_notify, NULL);
		if (ACPI_FAILURE(status)) {
	    	
		return -ENODEV;
		}

	err = platform_driver_register(&positivo_platform_driver );	
	if (err) {
  		pr_err("Unable ro register platform driver\n");
                  //TODO para fazer esse retorno
	}
		
	positivo_platform_device = platform_device_alloc("positivo-wmi", -1);
	if (!positivo_platform_device){
              platform_driver_unregister(&positivo_platform_driver);
	}
	err = platform_device_add(positivo_platform_device);
	if (err) platform_device_put(positivo_platform_device);
	return 0; 
} 

static void __exit oemwmi_end(void) 
{ 
	wmi_remove_notify_handler(WMI_EVENT_GUID);
	printk("oemwmi unload\n");
} 

module_init(oemwmi_start); 
module_exit(oemwmi_end); 

