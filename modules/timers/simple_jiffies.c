/*
 * Write module which print jiffies in init function,
 * make call to tasklet_schedule and print jiffies in
 * tasklet cb.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/types.h>

struct tasklet_struct bh;

static void tasklet_cb(unsigned long param)
{
	pr_info("Current jiffies in %s = %lu\n", __func__, jiffies);
}

static int __init load_module(void)
{
	pr_info("Current jiffies in %s = %lu\n", __func__, jiffies);

	tasklet_init(&bh, tasklet_cb, 0);
	tasklet_schedule(&bh);
	return 0;
}

static void __exit unload_module(void)
{
	/* Do we really need this? */
	tasklet_kill(&bh);
}

module_init(load_module);
module_exit(unload_module);

MODULE_AUTHOR("Nikolay Simonov");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
