#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/fs.h>
#include <linux/string.h>
//quando usuario tenta abrir sample-3s.mp3 ele vai abrir e tocar o harp.mp3

static struct kprobe kp = {
    .symbol_name = "do_sys_openat2",
};


static int handler_pre(struct kprobe *p, struct pt_regs *regs) {
    char __user *filename = (char __user *)regs->si; // Parâmetro filename (x86_64)
    char buf[256];
///home/usuario/Downloads/sample-3s.mp3
    // Copia o nome do arquivo do userspace
    if (strncpy_from_user(buf, filename, sizeof(buf)) < 0)
        return 0;

    // Se o arquivo for "som.mp3", substitui por "outro.mp3"
    if (strstr(buf, "sample-3s.mp3")) {
        char *new_filename = "/home/usuario/Downloads/harp.mp3";
        copy_to_user(filename, new_filename, strlen(new_filename) + 1);
        printk(KERN_INFO "Redirecionando %s -> %s\n", buf, new_filename);
    }

    return 0;
}
static int __init kprobe_init(void)
{
    kp.pre_handler = handler_pre;
    
    int ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("Failed to register kprobe: %d\n", ret);
        return ret;
    }
    
    pr_info("Kprobe installed for do_sys_openat2()\n");
    return 0;
}

static void __exit kprobe_exit(void)
{
    unregister_kprobe(&kp);
    pr_info("Kprobe uninstalled\n");
}

module_init(kprobe_init);
module_exit(kprobe_exit);
MODULE_LICENSE("GPL");
