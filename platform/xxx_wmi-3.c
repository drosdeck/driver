#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

#include <linux/acpi.h>
#include <linux/dmi.h>
#include <linux/input.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>

MODULE_AUTHOR("Edson Juliano Drosdeck <edson.drosdeck@gmail.com>");
MODULE_DESCRIPTION("platform driver");
MODULE_LICENSE("GPL");

struct sxxx_wmi{
	struct input_dev *input;
	struct acpi_hotplug_context hp;
};

struct sxxx_wmi xxx_wmi;

static struct platform_device *xxx_wmi_pdev;

static struct platform_driver xxx_wmi_driver = {
	.driver = {
		.name = "xxx-wmi",
//		.pm = &s3_wmi_pm,
	},
//	.remove = s3_wmi_remove,
};

static int xxx_wmi_check_platform_device(struct device *dev, void *data)
{
	struct acpi_device *adev;
	acpi_handle handle;
	acpi_status status;

	handle = ACPI_HANDLE(dev);
	if(!handle || acpi_bus_get_device(handle,&adev))
		return 0;
	printk("valosssssssssssssssr %s",acpi_device_hid(adev));

	return 0;
}
static int __init xxx_wmi_probe(struct platform_device *pdev)
{	
	int error;
	memset(&xxx_wmi,0,sizeof(xxx_wmi));

	bus_for_each_dev(&platform_bus_type,NULL,NULL,xxx_wmi_check_platform_device);

	return 0;
}
static int __init xxx_wmi_init(void)
{
	int error;

	xxx_wmi_pdev = platform_device_alloc("xxx-wmi", -1);
	if (!xxx_wmi_pdev)
		return -ENOMEM;

	error = platform_device_add(xxx_wmi_pdev);
	if (error)
		goto err_device_put;

	error = platform_driver_probe(&xxx_wmi_driver, xxx_wmi_probe);
	if (error)
		goto err_device_del;

	pr_info("xxx WMI Extras loaded\n");
	return 0;

 err_device_del:
	platform_device_del(xxx_wmi_pdev);
 err_device_put:
	platform_device_put(xxx_wmi_pdev);
	return error;
}

static void __exit xxx_wmi_exit(void)
{
	platform_device_unregister(xxx_wmi_pdev);
//	platform_driver_unregister(&xxx_wmi_driver);
}

module_init(xxx_wmi_init);
module_exit(xxx_wmi_exit);
