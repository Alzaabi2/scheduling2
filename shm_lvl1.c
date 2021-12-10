// Level 1: Implement the system calls to share memory between two processes, including
// mapping in the actual shared memory.
#include "shm_tests.h"

int
main()
{
	printf(1, "Shm Level 1 tests starting\n");
	helper_assert_n_shm_free(MAX_NUM_SHM);
    test_malloc_free();

	struct data *mem1;
	mem1    = (struct data *)shm_get("one");
	mem1->a = 1;
	mem1->b = 'b';
	mem1->m = "Hello";

	if (fork() == 0) {
        test_malloc_free();
		if (shm_get("one") != (char *)mem1) { printf(1, "ERROR: mem2->a should be %p after fork()\n", mem1); }

		if (strcmp(mem1->m, "Hello") != 0) { print_test_err("test_shm", "Child error read char*"); }

		mem1->m = "World";
        test_malloc_free();
		exit();
	} else {
		wait();

		if (strcmp(mem1->m, "World") != 0) { print_test_err("test_shm", "Parent error read char*"); }
	}

	printf(1, "Shm Level 1 tests complete\n");
	exit();
}
