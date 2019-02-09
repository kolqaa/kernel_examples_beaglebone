#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

static int chardev_open(struct inode *node, struct file *fptr)
{
	return 0;
}

static int chardev_release(struct inode *node, struct file *fptr)
{
	return 0;
}

static struct file_operations fops = {
	.open = chardev_open,
	.read = chardev_read,
	.write = chardev_write,
	.release = chardev_release
};

static int __init chardev_init(void)
{
	return 0;
}


static void chardev_exit(void)
{

}

module_init(chardev_init);
module_exit(chardev_exit);
