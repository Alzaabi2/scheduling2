#include "types.h"
#include "stat.h"
#include "user.h"
#define EXEC_TEST_N 2
#define MAX_NUM_SHM 16
#define PGSIZE 4096
#define NUM_SHM_TESTS 5

char *shm_names[MAX_NUM_SHM] = {"one",  "two", "three",  "four",   "five",     "six",      "seven",   "eight",
                                "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen"};

char *uprogs[NUM_SHM_TESTS] = {"shm_lvl0", "shm_lvl1", "shm_lvl2", "shm_lvl3", "shm_exec_test"};

struct data {
	int   a;
	char  b;
	char *m;
};

void
print_test_err(char *test, char *msg, ...)
{
	printf(1, "ERROR(%s): %s\n", test, msg);
}

void
helper_create_n_shm(int n, void *refs[])
{
	for (int i = 0; i < n; i++) {
		if ((refs[i] = shm_get(shm_names[i])) == 0) {
			print_test_err("helper_create_n_shm", "failed to create shm");
		}
	}
}

void
helper_assert_n_shm_in_use(int n, void *refs[])
{
	for (int i = 0; i < n; i++) {
		if (shm_get(shm_names[i]) != (char *)refs[i]) {
			print_test_err("helper_assert_n_shm_in_use", "shm should be in use");
		}
	}
}

void
helper_remove_n_shm(int n)
{
	for (int i = 0; i < n; i++) {
		if ((shm_rem(shm_names[i])) == 0) { print_test_err("helper_remove_n_shm", "failed to remove shm"); }
	}
}

void
helper_populate_refs(int n, void *refs[])
{
	for (int i = 0; i < n; i++) { *(int *)refs[i] = i + 1; }
}

void
helper_assert_n_shm_free(int n)
{
	void *refs[n];
	helper_create_n_shm(n, refs);
	helper_remove_n_shm(n);
}

// Mallocs n pages for process and places pointers in refs
void
helper_malloc_n(int n, void* refs[])
{
	for (int i = 0; i < n; i++) {
		if ((refs[i] = malloc(PGSIZE)) == 0) {
			print_test_err("helper_malloc_n", "unable malloc()");
		}
	}
}

// Free n pages for process held in refs
void
helper_free_n(int n, void* refs[])
{
	for (int i = 0; i < n; i++) {
		free(refs[i]);
	}
}

void 
test_shm_invalid_input_name_ob()
{
	if (shm_get("thisnameexceedstheshmlimitsoitmustthrowanderror") != 0) {
		print_test_err("test_shm_invalid_input_name_ob", "does not return correct value");
	}
}

void
test_shm_rem(int n)
{
	void *refs[n];

	helper_create_n_shm(n, refs);

	helper_assert_n_shm_in_use(n, refs);

	helper_populate_refs(n, refs);

	helper_assert_n_shm_in_use(n, refs);

	helper_remove_n_shm(n);

	for (int i = 0; i < n; i++) {
		if (shm_get(shm_names[i]) == 0) {
			printf(1, "ERROR(test_shm_rem): should be able to allocate shm for name: %s\n", shm_names[i]);
		}
	}

	helper_remove_n_shm(n);

	printf(1, "Test test_shm_rem complete\n");
}

void
test_shm_rem_robust()
{
	for (int i = 1; i <= 16; i++) { test_shm_rem(i); }
	printf(1, "Test test_shm_rem_robust complete\n");
}

// Creates n shm pages, forks and tests for shm inheritance for those n shm pages.
void
test_fork_inheritance(int n)
{
	void *refs[n];
	void *malloc_refs[n];
	helper_malloc_n(n, malloc_refs);
	helper_create_n_shm(n, refs);
	helper_free_n(n, malloc_refs);

	shm_get((char *)0);

	if (fork() == 0) {

		helper_malloc_n(n, malloc_refs);
		helper_assert_n_shm_in_use(n, refs);
		helper_free_n(n, malloc_refs);

		helper_remove_n_shm(n);

		for (int i = 0; i < n; i++) {
			if (shm_get(shm_names[i]) == 0) {
				print_test_err("test_fork_inheritance", "should be able to allocate shm");
			}
		}
		
		helper_remove_n_shm(n);

		exit();
	} else {
		wait();

		helper_assert_n_shm_in_use(n, refs);
	}
	helper_remove_n_shm(n);
	printf(1, "Test test_fork_inheritance complete\n");
}

void
test_fork_inheritance_robust()
{
	for (int i = 1; i <= 16; i++) { test_fork_inheritance(i); }
	printf(1, "Test test_fork_inheritance_robust complete\n");
}

void
test_simple_fork()
{
	struct data *mem1;

	mem1    = (struct data *)shm_get("one");
	mem1->a = 1;
	mem1->b = 'b';
	
	void* malloc_refs[MAX_NUM_SHM];
	helper_malloc_n(MAX_NUM_SHM, malloc_refs);

	if (sbrk(4096) == 0) {
		print_test_err("test_simple_fork", "sbrk() returns invalid after calling shm_get()");
		exit();
	}

	if (mem1->a != 1) { printf(1, "ERROR: mem1->a should be %d, got: %d\n", 1, mem1->a); }

	if (mem1->b != 'b') { printf(1, "ERROR: mem1->b should be %c, got: %c\n", 'b', mem1->b); }

	if (fork() == 0) {
		if (shm_get("one") != (char *)mem1) {
			print_test_err("test_simple_fork", " shm_get() must return 0 for allocated shared mem");
		}

		exit();
	} else {
		wait();

		// We should still have access to the memory.
		if (mem1->a != 1) { printf(1, "ERROR: mem1->a should be %d, got: %d\n", 2, mem1->a); }

		if (mem1->b != 'b') { printf(1, "ERROR: mem1->b should be %c, got: %c\n", 'c', mem1->b); }
	}

	helper_free_n(MAX_NUM_SHM, malloc_refs);

	shm_rem("one");
}

void
test_shm_rm_invalid_name()
{
	for (int i = 0; i < MAX_NUM_SHM; i++) {
		if (shm_rem(shm_names[i]) != -1) {
			printf(1, "ERROR(test_shm_rm_invalid_name): Should return -1 on shm_rem for %s\n",
			       shm_names[i]);
		}
	}
}

// Test that we are able to malloc and free memory successfully
// Will call malloc n times for pgsize and subsequently free.
void
test_malloc_free()
{
	void *refs[MAX_NUM_SHM];
	for (int i = 0; i < MAX_NUM_SHM; i++) {
		if ((refs[i] = malloc(PGSIZE)) == 0) { print_test_err("test_malloc_free", "malloc failed"); }
	}

	for (int i = 0; i < MAX_NUM_SHM; i++) { free(refs[i]); }
}
