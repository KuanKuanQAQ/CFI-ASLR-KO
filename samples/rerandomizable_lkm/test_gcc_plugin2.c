#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/delay.h>

#include <rerandom_driver.h>

extern int *global_data_pointer;
extern int global_data;
extern void (*global_function_pointer)(void);

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
	pr_info("%s %d random function addr:%lx\n", __FUNCTION__,
	       __LINE__, (long unsigned int)&random_function);
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
