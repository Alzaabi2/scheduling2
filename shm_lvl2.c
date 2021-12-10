// Level 2:
#include "shm_tests.h"

void test_fork() {
   	struct data *mem1;
	mem1    = (struct data *)shm_get("one");
	mem1->a = 1;
	mem1->b = 'b';

	if (sbrk(PGSIZE) == 0) {
		printf(1, "ERROR: sbrk() returns invalid after calling shm_get()\n");
		exit();
	}

	if (mem1->a != 1) { printf(1, "ERROR: mem1->a should be %d, got: %d\n", 1, mem1->a); }

	if (mem1->b != 'b') { printf(1, "ERROR: mem1->b should be %c, got: %c\n", 'b', mem1->b); }

	if (fork() == 0) {
		if (sbrk(PGSIZE) == 0) { printf(1, "ERROR: sbrk() returns invalid after calling shm_get()\n"); }

		if (mem1->a != 1) { printf(1, "ERROR: mem2->a should be %d, got: %d\n", 1, mem1->a); }

		if (mem1->b != 'b') { printf(1, "ERROR: mem2->b should be %c, got: %c\n", 'b', mem1->b); }

		mem1->a++;
		mem1->b = 'c';

		if (mem1->a != 2) { printf(1, "ERROR: mem2->a should be %d, got: %d\n", 2, mem1->a); }

		if (mem1->b != 'c') { printf(1, "ERROR: mem2->b should be %c, got: %c\n", 'c', mem1->b); }

		if (fork() == 0) {
			if (sbrk(PGSIZE) == 0) { printf(1, "ERROR: sbrk() returns invalid after calling shm_get()\n"); }

			if (mem1->a != 2) { printf(1, "ERROR: mem2->a should be %d, got: %d\n", 2, mem1->a); }

			if (mem1->b != 'c') { printf(1, "ERROR: mem2->b should be %c, got: %c\n", 'c', mem1->b); }

			mem1->a = 1;
			mem1->b = 'b';
	
		} else {
			wait();
		}

		exit();
	} else {
		wait();

		// We should still have access to the memory.
		if (mem1->a != 1) { printf(1, "ERROR: mem1->a should be %d, got: %d\n", 1, mem1->a); }

		if (mem1->b != 'b') { printf(1, "ERROR: mem1->b should be %c, got: %c\n", 'b', mem1->b); }
	} 
}

void
test_exit_shm_cleanup()
{
	void* refs[MAX_NUM_SHM];
	if (fork() == 0) {
		helper_create_n_shm(MAX_NUM_SHM, refs);
		exit();
	} else {
		wait();
		helper_create_n_shm(MAX_NUM_SHM, refs);
		helper_assert_n_shm_in_use(MAX_NUM_SHM, refs);
		helper_remove_n_shm(MAX_NUM_SHM);
	} 	
}

int
main()
{
	printf(1, "Starting Level 2 Tests\n");
	helper_assert_n_shm_free(MAX_NUM_SHM);
	test_simple_fork();
    test_fork();
	test_exit_shm_cleanup();

	printf(1, "Level 2 shm Tests Complete\n");
	exit();
}
