#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
	return fork();
}

int
sys_exit(void)
{
	exit();
	return 0; // not reached
}

int
sys_wait(void)
{
	return wait();
}

int
sys_kill(void)
{
	int pid;

	if (argint(0, &pid) < 0) return -1;
	return kill(pid);
}

int
sys_getpid(void)
{
	return myproc()->pid;
}

int
sys_sbrk(void)
{
	int addr;
	int n;

	if (argint(0, &n) < 0) return -1;
	addr = myproc()->sz;
	if (growproc(n) < 0) return -1;
	return addr;
}

int
sys_sleep(void)
{
	int  n;
	uint ticks0;

	if (argint(0, &n) < 0) return -1;
	acquire(&tickslock);
	ticks0 = ticks;
	while (ticks - ticks0 < n) {
		if (myproc()->killed) {
			release(&tickslock);
			return -1;
		}
		sleep(&ticks, &tickslock);
	}
	release(&tickslock);
	return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
	uint xticks;

	acquire(&tickslock);
	xticks = ticks;
	release(&tickslock);
	return xticks;
}

int
sys_prio_set(void)
{
	int pid, priority;
  if(argint(0, &pid) < 0 || argint(1, &priority) < 0)
    return -1;

  return prio_set(pid, priority);
}

int
sys_prio_get(void)
{
	//int pid; 

	// if (argint(0, (int *)&pid) < 0)
	// {return -1;}

	return prio_get(0);
}


int
sys_procstat(void)
{
	unsigned which;
	struct pstat *ps;
    if (argint(0, (int *) &which) < 0 || argptr(1, (void *)&ps, sizeof(ps)) < 0){return -1;} 
    return procstat(which, ps);
}

int
sys_schedpush(void)
{
	struct proc *p;
	if(argptr(0, (void *)&p, sizeof(struct proc)) < 0) {return -1;}
	return schedpush(p);
}

int 
sys_schedpop(void)
{
	return schedpop();
}

int
sys_testschedpush(void)
{
	return testschedpush();
}

void
sys_pSchedArr(void)
{
	pSchedArr();
}

void
sys_testingMode(void)
{
	testingMode();
	return;
}