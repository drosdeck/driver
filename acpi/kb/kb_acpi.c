/*
 Este e um exemplo de  um drive que ao pressionar a tecla c e  chama a funcao que envia um comando para acpi
 o comando  nao serve para todos os modelos, ele depende da bios
*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/string.h>
#include <linux/acpi.h>

#define KB_IRQ 1


//const char *schemas = "\\_SB.PCI0.SBRG.EC0._Q14";
const char *schemas = "\\_SB.PCI0.LPCB.H_EC._Q54";

static char result_buffer[256];

struct file* log_fp;
loff_t log_offset;
struct task_struct *logger;
void funcao(void);
int key_C = 174;

/* Stores information for logging. As of now, only the scancode is needed */
struct logger_data{
	unsigned char scancode;
} ld;

/* =================================================================== */
/* Converts scancode to key and writes it to log file. */
void tasklet_logger(unsigned long data)
{
	static int shift = 0;
	
	char buf[32];
	memset(buf, 0, sizeof(buf));
	/* Convert scancode to readable key and log it. */
	if ( ld.scancode == 174){
		funcao();
	}
}

/* Registers the tasklet for logging keys. */
DECLARE_TASKLET(my_tasklet, tasklet_logger, 0);

/* ISR for keyboard IRQ. */
irq_handler_t kb_irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
	/* Set global value to the received scancode. */
	ld.scancode = inb(0x60);

	/* We want to avoid I/O in an ISR, so schedule a Linux tasklet to
	 * write the key to the log file at the next available time in a 
	 * non-atomic context.
	 */
	tasklet_schedule(&my_tasklet);
	
	return (irq_handler_t)IRQ_HANDLED;
}

void funcao(void){
	union acpi_object *args;
	int nargs, i;
    	char *method;
	acpi_status status;
	acpi_handle handle;
         struct acpi_object_list arg;
    struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
   
	printk("entrou no metodo\n");
	status = acpi_get_handle(NULL, (acpi_string) schemas, &handle);

    if (ACPI_FAILURE(status))
    {
        snprintf(result_buffer, 256, "Error: %s", acpi_format_exception(status));
        printk(KERN_ERR "acpi_call: Cannot get handle: %s\n", result_buffer);
        return;
    }


    status = acpi_evaluate_object(handle, NULL, &arg, &buffer);
    if (ACPI_FAILURE(status))
    {
        snprintf(result_buffer, 256, "Error: %s", acpi_format_exception(status));
        printk(KERN_ERR "acpi_call: Method call failed: %s\n", result_buffer);
        return;
    }


}
/* Module entry point. */
static int __init kb_init(void)
{
	int ret;


	/* Request to register a shared IRQ handler (ISR). */
	ret = request_irq(KB_IRQ, (irq_handler_t)kb_irq_handler, IRQF_SHARED,
			"OEM", &ld);
	if(ret != 0){
		printk(KERN_INFO "FAILED to request IRQ for keyboard.\n");
	}

	return ret;
}

/* On module exit. */
static void __exit kb_exit(void)
{
	/* Free the logging tasklet. */
	tasklet_kill(&my_tasklet);

	/* Free the shared IRQ handler, giving system back original control. */
	free_irq(KB_IRQ, &ld);

}

MODULE_LICENSE("GPL");
module_init(kb_init);
module_exit(kb_exit);
