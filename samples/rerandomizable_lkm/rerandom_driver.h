#include <linux/ktime.h>

struct Rerandom_Driver {
	const char		   *name;
	void (*init_entry)(void);
	int (*check_entry)(int, char*, time64_t);
};

void local_random_data_pointer_callee(int *local_pointer);
void local_random_data_pointer(void);
void global_random_data_pointer(void);
void random_function(void);
void local_random_function_pointer_callee(void (*func)(void));
void local_random_function_pointer(void);
void global_random_function_pointer(void);
