// SPDX-License-Identifier: UNLICENSED
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Oleg Tsuria IO-21 <tsurya.oleg@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_PARM_DESC(counter, "Counter of 'Hello, world!'");

static unsigned int counter = 1;
module_param(counter, uint, 0444);

struct tOfiter {
	struct list_head list;
	ktime_t iterMark;
};

static LIST_HEAD(iterList);

static int __init hello_init(void)
{
	unsigned int i = 0;

	if (counter == 0 || (counter >= 5 && counter <= 10))
		pr_warn("Warning: Continuing\n");

	if (counter > 10) {
		pr_err("Error: EINVAL (bad value)!\n");
		// Module load error
		return -EINVAL;
	}

	// Print "Hello, world!" counter times
	for (i = 0; i < counter; i++) {
		//Working with list and memory
		struct tOfiter *iter = kmalloc(sizeof(*iter),
			 GFP_KERNEL);
		iter->iterMark = ktime_get();
		list_add_tail(&iter->list, &iterList);
		pr_info("Hello, world!\n");
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct tOfiter *iter;
	struct list_head *pos,  *tmp;

	// Travel the list
	list_for_each_safe(pos, tmp, &iterList) {
		iter = list_entry(pos, struct tOfiter, list);
		pr_info("Time: %lld ns\n",
			 ktime_to_ns(iter->iterMark));
		list_del(pos);
		kfree(iter);
	}
}

module_init(hello_init);
module_exit(hello_exit);
