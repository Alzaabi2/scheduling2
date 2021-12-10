// Level 0: System calls and kernel data-structure to track SHM_MAXNUM shared memory
// regions (tracking processes, and virtual memory addresses the regions are mapped
// into).
#include "shm_tests.h"

int
main()
{
	printf(1, "Shm Level 0 Tests Starting\n");
	helper_assert_n_shm_free(MAX_NUM_SHM);
	test_shm_rm_invalid_name();
    test_malloc_free();

	char *mem1, *mem2, *mem3;
	mem1 = shm_get("one");
	printf(1, "mem1: %p\n", (void *)mem1);
	mem2 = shm_get("two");
	printf(1, "mem2: %p\n", (void *)mem2);
	mem3 = shm_get("three");
	printf(1, "mem3: %p\n", (void *)mem3);
    test_malloc_free();

	if (shm_get("one") != mem1) {
		printf(1, "ERROR: shm_get should return %p for duplicate return values.\n", mem1);
	}

	if (shm_get("two") != mem2) {
		printf(1, "ERROR: shm_get should return %p for duplicate return values.\n", mem2);
	}

	if (shm_get("three") != mem3) {
		printf(1, "ERROR: shm_get should return %p for duplicate return values.\n", mem3);
	}

	if (fork() == 0) {
		mem1 = shm_get("four");
		printf(1, "mem1: %p\n", (void *)mem1);
		mem2 = shm_get("five");
		printf(1, "mem2: %p\n", (void *)mem2);
		mem3 = shm_get("six");
		printf(1, "mem3: %p\n", (void *)mem3);

		if (shm_get("four") != mem1) {
			printf(1, "ERROR: shm_get should return %p for duplicate return values.\n", mem1);
		}

		if (shm_get("five") != mem2) {
			printf(1, "ERROR: shm_get should return %p for duplicate return values.\n", mem2);
		}

		if (shm_get("six") != mem3) {
			printf(1, "ERROR: shm_get should return %p for duplicate return values.\n", mem3);
		}

        test_malloc_free();

		exit();
	} else {
		wait();
        test_malloc_free();

		if (shm_get("one") != mem1) {
			printf(1, "ERROR: shm_get should return 0 for duplicate return values.\n");
		}

		if (shm_get("two") != mem2) {
			printf(1, "ERROR: shm_get should return 0 for duplicate return values.\n");
		}

		if (shm_get("three") != mem3) {
			printf(1, "ERROR: shm_get should return 0 for duplicate return values.\n");
		}

		if (shm_get("four") == 0) {
			printf(1, "ERROR: shm_get should not return 0 for shm created in child\n");
		}

		if (shm_get("five") == 0) {
			printf(1, "ERROR: shm_get should not return 0 for shm created in child\n");
		}

		if (shm_get("six") == 0) { printf(1, "ERROR: shm_get should not return 0 for shm created in child\n"); }
        test_malloc_free();
	}

    test_malloc_free();
	test_shm_invalid_input_name_ob();
	printf(1, "Shm Level 0 Tests Complete\n");
	exit();
}
