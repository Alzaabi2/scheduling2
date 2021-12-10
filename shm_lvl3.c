// Level 3:
// Thoroughly test that your shared memory region is properly maintained, or
// deallocated, depending on which processes fork, exit, and exec. This will require
// reference counting on the shared memory region, and you only deallocate the memory
// for it when all threads accessing it exit or unmap it.
#include "shm_tests.h"

void
test_exec()
{
	if (fork() == 0) {
		char *args = "none";
		if (exec(uprogs[4], &args) == -1) { printf(1, "exec failed\n"); }
		exit();
	} else {
		wait();
	}

	if (fork() == 0) {
		char *args = "none";
		if (exec(uprogs[4], &args) == -1) { printf(1, "exec failed\n"); }
		exit();
	} else {
		wait();
	}

	printf(1, "Test test_exec complete\n");
}

void 
test_when_all_shm_used_return_err()
{
	void* refs[MAX_NUM_SHM];
	helper_create_n_shm(MAX_NUM_SHM, refs);
	
	if (shm_get("UNUSED_MEM") != 0) {
		print_test_err("test_when_all_shm_used_return_err", "should return 0");
	}

	helper_remove_n_shm(MAX_NUM_SHM);
}

int
main()
{
	printf(1, "Shm Level 3 Tests Starting\n");
	void *malloc_refs[MAX_NUM_SHM];
	helper_malloc_n(MAX_NUM_SHM, malloc_refs);

	helper_assert_n_shm_free(MAX_NUM_SHM);
	test_shm_rem_robust();
	test_fork_inheritance_robust();
	test_exec();

	helper_free_n(MAX_NUM_SHM, malloc_refs);
	test_when_all_shm_used_return_err();
	printf(1, "Shm Level 3 Tests Complete\n");
	exit();
}
