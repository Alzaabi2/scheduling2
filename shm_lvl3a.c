#include "shm_tests.h"

void
test_exec()
{
	for (int i = 0; i < NUM_SHM_TESTS; i++) {
		if (fork() == 0) {
			char *args = "none";
			if (exec(uprogs[i], &args) == -1) { printf(1, "exec failed!\n"); }
			exit();
		} else {
			wait();
		}
	}
}

void
test_exec_for_lvl(int lvl_idx)
{
	for (int i = 0; i < 20; i++) {
		if (fork() == 0) {
			char *args = "none";
			if (exec(uprogs[lvl_idx], &args) == -1) { printf(1, "exec failed!\n"); }
			exit();
		} else {
			wait();
		}
	}
}

int
main()
{
	void *refs[EXEC_TEST_N];
	helper_create_n_shm(EXEC_TEST_N, refs);
	test_exec();
	helper_assert_n_shm_in_use(EXEC_TEST_N, refs);
	exit();
}
