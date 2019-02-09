/*
 * Write module which start timer 5 times with interval of 1 sec
 * and print jiffies in timer cb. Make a call inside timer cb to
 * tasklet_schedule and in tasklet cb, also print jiffies.
 *
 * This example use new API for timers which were introduced in 4.15
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/types.h>

struct dev_data {
	struct timer_list timer;
	struct tasklet_struct bh;
	u8 boundary;
} dev = { .boundary = 5 };

static void tasklet_cb(unsigned long param)
{
	pr_info("Current jiffies in %s = %lu", __func__, jiffies);
}

static void timer_cb(struct timer_list *t)
{
	struct dev_data *_dev = from_timer(_dev, t, timer);
	static u8 entry = 1;

	pr_info("Current jiffies in %s = %lu", __func__, jiffies);

	if (entry < _dev->boundary) {
		tasklet_schedule(&_dev->bh);
		mod_timer(&_dev->timer, jiffies + 1 * HZ);
		entry++;
	}
}

static int __init load_module(void)
{
	timer_setup(&dev.timer, timer_cb, 0);
	mod_timer(&dev.timer, jiffies + 1 * HZ);

	tasklet_init(&dev.bh, tasklet_cb, 0);
	return 0;
}

static void __exit unload_module(void)
{
	del_timer_sync(&dev.timer);

	/* Do we really need this? */
	tasklet_kill(&dev.bh);
}

module_init(load_module);
module_exit(unload_module);

MODULE_AUTHOR("Nikolay Simonov");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
