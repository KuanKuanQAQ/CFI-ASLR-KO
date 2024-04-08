#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#include <rerandom_driver.h>

static struct task_struct *trampoline_test_kthread = NULL;

char default_name[] = "default_name";

void empty_func(void)
{
	return;
}

int empty_check_func(int a, char *c, time64_t time)
{
	pr_info("This is an empty func.\n");
	return 0;
}

struct Rerandom_Driver rerandom_driver = {
	.name = default_name,
	.init_entry = empty_func,
	.check_entry = empty_check_func,
};

void register_rerandom_driver(
	const struct Rerandom_Driver *rerandom_driver_struct)
{
	rerandom_driver.name = rerandom_driver_struct->name;
	rerandom_driver.init_entry = rerandom_driver_struct->init_entry;
	rerandom_driver.check_entry = rerandom_driver_struct->check_entry;
}
EXPORT_SYMBOL_GPL(register_rerandom_driver);

void register_rerandom_driver_param(void (*init_entry)(void),
				    int (*check_entry)(int, char *, time64_t))
{
	rerandom_driver.init_entry = init_entry;
	rerandom_driver.check_entry = check_entry;
}
EXPORT_SYMBOL_GPL(register_rerandom_driver_param);

static int trampoline_test_kthread_run(void *p)
{
	time64_t time = ktime_get_seconds();
	int test_ret = -1;
	pr_info("Trampoline test kthread: kthread started.\n");
	for (;;) {
		msleep(1000);
		/* Periodically print the statistics */
		if (time < ktime_get_seconds()) {
			time = ktime_get_seconds() + 5;
			pr_info("************************\n");
			pr_info("**Jump into test func.**\n");
			pr_info("************************\n");
			rerandom_driver.init_entry();
			test_ret =
				rerandom_driver.check_entry(11, "chifan", time);
			pr_info("test_ret = %d\n", test_ret);
			pr_info("Out Function init_entry Address: %px\n",
			       rerandom_driver.init_entry);
			pr_info("Out Function check_entry Address: %px\n",
			       rerandom_driver.check_entry);
		}
	}
	return 0;
}

static int __init kthread_init(void)
{
	trampoline_test_kthread = kthread_run(trampoline_test_kthread_run, NULL,
					      "trampoline_test_kthread");
	if (!trampoline_test_kthread) {
		pr_info("kthread_run fail\n");
		return -ECHILD;
	}
	return 0;
}

static void __exit kthread_exit(void)
{
	if (trampoline_test_kthread) {
		pr_info("kthread_stop\n");
		kthread_stop(trampoline_test_kthread);
		trampoline_test_kthread = NULL;
	}
}

module_init(kthread_init);
module_exit(kthread_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Runkuan Li");
MODULE_DESCRIPTION("Kernel thread helps to test trampoline.");
MODULE_VERSION("1.0");
