#include "shm_tests.h"

int
main()
{
	printf(1, "Executing in shm_exec_test\n");
	helper_assert_n_shm_free(MAX_NUM_SHM);
	test_malloc_free();
	test_shm_rem_robust();
	test_simple_fork();
	test_fork_inheritance_robust();
	printf(1, "Test test_exec complete!\n");
	exit();
}
