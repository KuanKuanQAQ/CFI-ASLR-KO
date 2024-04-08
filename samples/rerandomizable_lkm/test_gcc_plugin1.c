#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/delay.h>

#include <rerandom_driver.h>

int *global_data_pointer;
int global_data = 1234;
void (*global_function_pointer)(void);

void init_entry(void)
{
	pr_info("**************** init_entry ****************\n");
	local_random_data_pointer();
	global_random_data_pointer();

	local_random_function_pointer();
	global_random_function_pointer();
	pr_info("**************** init_entry end ****************\n");
}

int check_entry(int a, char *c, time64_t time)
{
	// Check global variants
	mdelay(10000);
	pr_info("**************** check_entry ******************\n");
	pr_info("%s %d global data pointer addr %lx global_data addr %lx\n",
	       __FUNCTION__, __LINE__, (long unsigned int)&global_data_pointer,
	       (long unsigned int)&global_data);
	pr_info("%s %d global data pointer value: %lx\n", __FUNCTION__, __LINE__,
	       (long unsigned int)global_data_pointer);
	pr_info("%s %d global data pointer point value %d\n", __FUNCTION__,
	       __LINE__, (int)*global_data_pointer);
	pr_info("time is %lld \n", time);
	// Check function pointer
	global_function_pointer();
	pr_info("**************** check_entry end ******************\n");
	return a;
}

struct Rerandom_Driver rerandom_driver_struct = {};

extern void register_rerandom_driver(struct Rerandom_Driver *driver);
extern void register_rerandom_driver_param(void (*init_entry)(void),
					   int (*check_entry)(int, char *,
							      time64_t));

static int __init random_test_driver_init(void)
{
	rerandom_driver_struct.name = "rerandom_test";
	rerandom_driver_struct.init_entry = &init_entry;
	rerandom_driver_struct.check_entry = &check_entry;
	// register_rerandom_driver(&rerandom_driver_struct);
	register_rerandom_driver_param(init_entry, check_entry);

	return 0;
}

static void __exit random_test_driver_exit(void)
{
	return;
}

module_init(random_test_driver_init);
module_exit(random_test_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Runkuan Li");
MODULE_DESCRIPTION("Rerandomizable LKM");
MODULE_VERSION("1.0");
