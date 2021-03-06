#include <linux/module.h>	 /* Needed by all modules */ 
#include <linux/kernel.h>	 /* Needed for KERN_INFO */ 
#include <linux/init.h>	 /* Needed for the macros */ 
//#include <linux/input/sparse-keymap.h>
//#include <linux/platform_device.h>
#include <linux/acpi.h>
//#include <linux/rfkill.h>
#include <linux/string.h>


MODULE_LICENSE("GPL"); 

MODULE_AUTHOR("Edson Juliano Drosdeck"); 

MODULE_DESCRIPTION("A simple wmi!"); 

MODULE_VERSION("0.1"); 


#define EEEPC_WMI_EVENT_GUID "ABBC0F72-8EA1-11D1-00A0-C90629100000"

static void eeepc_wmi_notify(u32 value, void *context)
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
               printk("Codigo %d\n",code);
    //	    if (!sparse_keymap_report_event(eeepc_wmi_input_dev, code, 1, true))
//		pr_info("Unknown key %x pressed\n", code);
	}

	kfree(obj);
    }



static int __init hello_start(void) 
{
	int status;
 
	printk(KERN_INFO "Loading hello module...\n"); 
	printk(KERN_INFO "Hello world\n"); 


        if (!wmi_has_guid(EEEPC_WMI_EVENT_GUID)) {
	    printk("No known WMI GUID found\n");
	    return -ENODEV;
	}

	status = wmi_install_notify_handler(EEEPC_WMI_EVENT_GUID,
					eeepc_wmi_notify, NULL);
		if (ACPI_FAILURE(status)) {
	    	
		return -ENODEV;
		}
	return 0; 
} 

static void __exit hello_end(void) 
{ 
	printk(KERN_INFO "Goodbye Mr.\n"); 
       wmi_remove_notify_handler(EEEPC_WMI_EVENT_GUID);
} 

module_init(hello_start); 
module_exit(hello_end); 

