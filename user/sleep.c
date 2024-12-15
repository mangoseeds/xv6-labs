// See kernel/sysproc.c for the xv6 kernel code that implements the sleep system call (look for sys_sleep), 
// user/user.h for the C definition of sleep callable from a user program, 
// and user/usys.S for the assembler code that jumps from user code into the kernel for sleep.

#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	// check how many arguments are provided
	if (argc != 2) {
		fprintf(2, "usage: sleep <ticks>\n");
		exit(1);
	}

	int ticks = atoi(argv[1]); // atoi() returns 0 if not valid conversion
	
	if (ticks < 0) {
		fprintf(2, "sleep: negative ticks not allowed\n");
		exit(1);
	}
	
	sleep(ticks);
	exit(0);
}
