#include <linux/module.h>     /* Needed by all modules */ 
#include <linux/kernel.h>     /* Needed for KERN_INFO */ 
#include <linux/init.h>       /* Needed for the macros */ 
#include <linux/input.h>
#include <linux/types.h>
//#include <acpi/acpi_bus.h>
//#include <acpi/acpi_drivers.h>  
#include <linux/acpi.h>


#define ACPI_BUTTON_CLASS "button"
#define ACPI_BUTTON_NAME "Power button"

#define ACPI_BUTTON_CLASS		"button"
#define ACPI_BUTTON_SUBCLASS_POWER	"power"

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

struct acpi_button {
	unsigned int type;
	struct input_dev *input;
	char phys[32];			/* for input device */
	unsigned long pushed;
	bool suspended;
};

static int  acpi_power_button_add(struct acpi_device *device)
{
	struct acpi_button *button;
	struct input_dev *input;
	char *name;
	char *class;
	int error;

	const char *hid = acpi_device_hid(device);
	//PNP0C0C
	printk("valor do hiddddd %s\n",hid);

	//button = kzalloc(sizeof(struct acpi_button),GFP_KERNEL);
	button = kzalloc(sizeof(struct acpi_button), GFP_KERNEL);
	if (!button)
		return -ENOMEM;
        device->driver_data = button;
	button->input = input = input_allocate_device();
	if(!input){
		error = -ENOMEM;
		//desaloca a memoria do kzalloc
		goto err_free_button;
	}
        // Power Button	
	name = acpi_device_name(device);
	class = acpi_device_class(device);	
	
	printk("valor no nome %s\n",name);
	printk("valor da class %s\n",class);
	
	
         //#define ACPI_BUTTON_TYPE_POWER		0x01
	button->type=0x01; 
        strcpy(name,"Power Button");
	sprintf(class,"button/power");

	snprintf(button->phys,sizeof (button->phys), "%s/button/input0",hid);

	input->name = name;
	input->phys = button->phys;
	input->id.bustype = BUS_HOST;
	input->id.product = 0x01;
	input->dev.parent = &device->dev;
        //entender a funcao abaixo
	input_set_capability(input,EV_KEY,KEY_POWER);
	input_set_drvdata(input,device);

	input_register_device(input);
        
	device_init_wakeup(&device->dev,true);


	return 0;
err_free_button:
	kfree(button);
}

static int acpi_power_button_remove(struct acpi_device *device)
 {
	 struct acpi_button *button = acpi_driver_data(device);

 	input_unregister_device(button->input);
	kfree(button);	
	 return 0;
}

static struct acpi_driver power_button_driver = {
        .name =         "button",
        .class =        ACPI_BUTTON_CLASS,
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
