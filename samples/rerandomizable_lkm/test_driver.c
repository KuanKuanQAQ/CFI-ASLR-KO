#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/delay.h>

int *global_data_pointer;
int global_data = 1234;
void (*global_function_pointer)(void);

void local_random_data_pointer_callee(int *local_pointer)
{
	pr_info("%s %d local_pinter value: %lx local_pointer_addr: %lx value: %d\n",
	       __FUNCTION__, __LINE__, (long unsigned int)local_pointer,
	       (long unsigned int)&local_pointer, (int)*local_pointer);
}

void local_random_data_pointer(void)
{
	int local_data = 10;
	local_random_data_pointer_callee(
		&local_data); // Passing parameters in stack
}

void global_random_data_pointer(void)
{
	global_data_pointer = &global_data;
}

void random_function(void)
{
	pr_info("%s %d random function addr:%lx\n", __FUNCTION__, __LINE__,
	       (long unsigned int)&random_function);
}

void local_random_function_pointer_callee(void (*func)(void))
{
	func();
}

void local_random_function_pointer(void)
{
	void (*local_function_pointer)(void) = &random_function;
	local_random_function_pointer_callee(local_function_pointer);
}

void global_random_function_pointer(void)
{
	global_function_pointer = &random_function;
}

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

static int __init random_test_driver_init(void)
{
	init_entry();
	check_entry(11, "chifan", ktime_get_seconds());
	return 0;
}

static void __exit random_test_driver_exit(void)
{
	return;
}


module_init(random_test_driver_init);
module_exit(random_test_driver_exit);

MODULE_LICENSE("GPL");
MODULE_INFO(shuffle, "1");
MODULE_AUTHOR("Runkuan Li");
MODULE_DESCRIPTION("Rerandomizable LKM");
MODULE_VERSION("1.0");
