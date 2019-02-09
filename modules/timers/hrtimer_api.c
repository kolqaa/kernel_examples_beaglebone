#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
#include <linux/types.h>

#define SEC 1e9

struct dev_data {
	struct hrtimer hr_timer;
	struct tasklet_struct bh;
	u8 boundary;
} dev = { .boundary = 5 };


static void tasklet_cb(unsigned long param)
{
	pr_info("Current jiffies %lu in %s\n", jiffies, __func__);
}

static enum hrtimer_restart hrtimer_cb(struct hrtimer *timer_for_restart)
{
	static u8 entry;
	ktime_t currtime, interval;
	enum hrtimer_restart hrtimer_state = HRTIMER_NORESTART;

	pr_info("Current jiffies %lu in %s\n", jiffies, __func__);

	currtime = ktime_get();
	interval = ktime_set(0, SEC);
	hrtimer_forward(timer_for_restart, currtime, interval);

	if (entry < dev.boundary) {
		hrtimer_state = HRTIMER_RESTART;
		tasklet_schedule(&dev.bh);
		entry++;
	} else {
		hrtimer_state = HRTIMER_NORESTART;
	}

	return hrtimer_state;
}

static int __init timer_init(void)
{
	ktime_t ktime = ktime_set(0, SEC);

	tasklet_init(&dev.bh, tasklet_cb, 0);

	hrtimer_init(&dev.hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	dev.hr_timer.function = hrtimer_cb;
	hrtimer_start(&dev.hr_timer, ktime, HRTIMER_MODE_REL);
	return 0;
}

static void __exit timer_exit(void)
{
	int ret;

	tasklet_kill(&dev.bh);
	ret = hrtimer_cancel(&dev.hr_timer);
	if (ret)
		pr_info("The timer was still in use...\n");
	else
		pr_info("HR Timer module uninstalling\n");

}

module_init(timer_init);
module_exit(timer_exit);

MODULE_AUTHOR("Nikolay Simonov");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
