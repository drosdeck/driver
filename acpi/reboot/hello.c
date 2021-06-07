#include <linux/module.h>	 /* Needed by all modules */
#include <linux/kernel.h>	 /* Needed for KERN_INFO */
#include <linux/init.h>	 /* Needed for the macros */
#include <linux/acpi.h>

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Edson Drosdeck");

MODULE_DESCRIPTION("A simple reboot");

MODULE_VERSION("0.1");

static int __init hello_start(void)
{
	u16 teste;
	struct acpi_generic_address *reset_reg;
	acpi_status status;

	reset_reg = &acpi_gbl_FADT.reset_register;

	/* Check if the reset register is supported */

	if (!(acpi_gbl_FADT.flags & ACPI_FADT_RESET_REGISTER) ||
	    !reset_reg->address) {
		printk("nao suportado\n");
		return_ACPI_STATUS(AE_NOT_EXIST);
	}

	if (reset_reg->space_id == ACPI_ADR_SPACE_SYSTEM_IO) {
		/*
		 * For I/O space, write directly to the OSL. This bypasses the port
		 * validation mechanism, which may block a valid write to the reset
		 * register.
		 *
		 * NOTE:
		 * The ACPI spec requires the reset register width to be 8, so we
		 * hardcode it here and ignore the FADT value. This maintains
		 * compatibility with other ACPI implementations that have allowed
		 * BIOS code with bad register width values to go unnoticed.
		 */
		status = acpi_os_write_port((acpi_io_address)reset_reg->address,
					    acpi_gbl_FADT.reset_value,
					    ACPI_RESET_REGISTER_WIDTH);
	} 	return 0;
}

static void __exit hello_end(void)
{
	printk(KERN_INFO "Goodbye Mr.\n");
}

module_init(hello_start);
module_exit(hello_end);

