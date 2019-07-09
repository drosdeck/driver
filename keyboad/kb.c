#include <linux/module.h>	 
#include <linux/kernel.h>	 
#include <linux/serio.h>
#include <linux/slab.h>
#include <linux/input.h>
#define DESC "Hello word serio"

struct kb {
	struct input_dev *dev;

};

static int kb_connect(struct serio *serio, struct serio_driver *drv){
	struct kb  *kb;
        struct input_dev *dev;
	kb = kzalloc(sizeof(struct kb), GFP_KERNEL);
	dev = input_allocate_device();

	if (!kb || !dev)
	goto fail1;


	return 0;

fail1: input_free_device(dev);
	kfree(kb);
	return -ENOMEM;
}

static int kb_reconnect(struct serio  *serio)
{

	return 0;
}

static void kb_disconnect(struct serio *serio)
{

	
}

static void kb_cleanup(struct serio *serio)
{

}

static const struct serio_device_id  kb_ids[] = {
	{	
		.type    = SERIO_8042,
		.proto   = SERIO_ANY,
	       	.id      = SERIO_ANY,
		.extra   = SERIO_ANY,	
	},
	{	
		.type    = SERIO_8042_XL,
		.proto   = SERIO_ANY,
	       	.id      = SERIO_ANY,
		.extra   = SERIO_ANY,	
	},
	{	
		.type    = SERIO_RS232,
		.proto   = SERIO_PS2SER,
	       	.id      = SERIO_ANY,
		.extra   = SERIO_ANY,	
	},

	{ 0 }

	

};
static struct serio_driver kb_drv = {
	.driver = {
		.name = "kb",
	},
	.description = DESC,
	.id_table    = kb_ids,
	.connect     = kb_connect,
	.reconnect   = kb_reconnect,
	.disconnect  = kb_disconnect,
	.cleanup     = kb_cleanup,
};


static int __init kb_start(void) 
{

	return serio_register_driver(&kb_drv);
} 


static void __exit kb_end(void) 
{ 
	serio_unregister_driver(&kb_drv);
	printk("Adios amigo\n");
} 

module_init(kb_start); 
module_exit(kb_end); 
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Edson Juliano Drosdeck"); 
MODULE_DESCRIPTION("Keyboard driver"); 
MODULE_VERSION("0.1"); 

