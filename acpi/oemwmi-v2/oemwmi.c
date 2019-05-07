#include <linux/module.h>	 
#include <linux/kernel.h>	 
#include <linux/init.h>	  
#include <linux/acpi.h>
#include <linux/string.h>

MODULE_LICENSE("GPL"); 

MODULE_AUTHOR("Edson Juliano Drosdeck"); 

MODULE_DESCRIPTION("Send wmi event key to acpi_listen"); 

MODULE_VERSION("2.0"); 


static char *WMI_EVENT_GUID = "ABBC0F72-8EA1-11D1-00A0-C90629100000";
module_param(WMI_EVENT_GUID,charp,0644);
MODULE_PARM_DESC(WMI_EVENT_GUID,"Wmi event guid");



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
        

        if (!wmi_has_guid(WMI_EVENT_GUID)) {
	    printk("No known WMI GUID found\n");
	    return -ENODEV;
	}

	status = wmi_install_notify_handler(WMI_EVENT_GUID,
					wmi_hotkey_notify, NULL);
		if (ACPI_FAILURE(status)) {
	    	
		return -ENODEV;
		}
		printk("oemwmi started \n");
	return 0; 
} 

static void __exit oemwmi_end(void) 
{ 
	wmi_remove_notify_handler(WMI_EVENT_GUID);
	printk("oemwmi unload\n");
} 

module_init(oemwmi_start); 
module_exit(oemwmi_end); 

