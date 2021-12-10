struct pstat {
	int pid, ppid;
	char state;
	char name[16];
    int prio;
};