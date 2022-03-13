#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <asm/io.h>  
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mod_devicetable.h>
   
/* Driver Name */
#define DRIVER_NAME "simple_axi_lite"

unsigned long *base_addr;
struct resource *res;
unsigned long remap_size;
 
static ssize_t proc_simple_axi_lite_write(struct file *file,
                                          const char __user * buf,
                                          size_t count, loff_t * ppos)
{
    char simple_axi_lite_phrase[16];
    u32 simple_axi_lite_value;

    if (count < 11) {
        if (copy_from_user(simple_axi_lite_phrase, buf, count))
            return -EFAULT;

       simple_axi_lite_phrase[count] = '\0';
   }

   simple_axi_lite_value = simple_strtoul(simple_axi_lite_phrase, NULL, 0);
   wmb();

   iowrite32(simple_axi_lite_value, base_addr);

   return count;
}

static int proc_simple_axi_lite_show(struct seq_file *p, void *v)
{
    u32 simple_axi_lite_value;
    simple_axi_lite_value = ioread32(base_addr);
    seq_printf(p, "0x%x", simple_axi_lite_value);
    return 0;
}

static int proc_simple_axi_lite_open(struct inode *inode, struct file *file)
{
    unsigned int size = 16;
    char *buf;
    struct seq_file *m;
    int res;

    buf = (char *)kmalloc(size * sizeof(char), GFP_KERNEL);
    if (!buf)
        return -ENOMEM;

    res = single_open(file, proc_simple_axi_lite_show, NULL);

    if (!res) {
        m = file->private_data;
        m->buf = buf;
        m->size = size;
    } else
        kfree(buf);

    return res;
}

static const struct proc_ops proc_simple_axi_lite_operations = {
    .proc_open = proc_simple_axi_lite_open,
    .proc_read = seq_read,
    .proc_write = proc_simple_axi_lite_write,
    .proc_lseek = seq_lseek,
    .proc_release = single_release
};

static void simple_axi_lite_shutdown(struct platform_device *pdev)
{
   iowrite32(0, base_addr);
}

static int simple_axi_lite_remove(struct platform_device *pdev)
{
    simple_axi_lite_shutdown(pdev);

    remove_proc_entry(DRIVER_NAME, NULL);

    iounmap(base_addr);

    release_mem_region(res->start, remap_size);

    return 0;
}

static int simple_axi_lite_probe(struct platform_device *pdev)
{
    struct proc_dir_entry *simple_axi_lite_proc_entry;
    int ret = 0;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        dev_err(&pdev->dev, "No memory resource\n");
        return -ENODEV;
    }
 
    remap_size = res->end - res->start + 1;
    if (!request_mem_region(res->start, remap_size, pdev->name)) {
        dev_err(&pdev->dev, "Cannot request IO region\n");
        return -ENXIO;
    }

    base_addr = ioremap(res->start, remap_size);
    if (base_addr == NULL) {
        dev_err(&pdev->dev, "Couldn't ioremap memory at 0x%08lx\n",
            (unsigned long)res->start);
        ret = -ENOMEM;
        goto err_release_region;
    }

    simple_axi_lite_proc_entry = proc_create(DRIVER_NAME, 0, NULL,
                                             &proc_simple_axi_lite_operations);
    if (simple_axi_lite_proc_entry == NULL) {
        dev_err(&pdev->dev, "Couldn't create proc entry\n");
        ret = -ENOMEM;
        goto err_create_proc_entry;
    }
 
    printk(KERN_INFO DRIVER_NAME " probed at VA 0x%08lx\n",
           (unsigned long)base_addr);
 
    return 0;
 
err_create_proc_entry:
    iounmap(base_addr);
err_release_region:
    release_mem_region(res->start, remap_size);
 
    return ret;
}
 
/* device match table to match with device node in device tree */
static const struct of_device_id simple_axi_lite_of_match[] = {
    {.compatible = "simple_axi_lite-1.00"},
    {},
};
 
MODULE_DEVICE_TABLE(of, simple_axi_lite_of_match);
 
static struct platform_driver simple_axi_lite_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
        .of_match_table = simple_axi_lite_of_match},
    .probe = simple_axi_lite_probe,
    .remove = simple_axi_lite_remove,
    .shutdown = simple_axi_lite_shutdown
};

module_platform_driver(simple_axi_lite_driver);

MODULE_AUTHOR("mikhail.khokhlov@gmail.com");
MODULE_LICENSE("GPL");
MODULE_ALIAS(DRIVER_NAME);

