#include "types.h"
#include "stat.h"
#include "user.h"

static int currentprio = 3;

void
fork_chain(int i)
{
    int id;
    if(i<0)
    {return;}
    id = fork();
    if(id == 0)
    {
    if(getpid() >= 10 && getpid() < 15) priorityset(getpid(), getpid()); // procs with pid less than 10 get parent pid, procs with more get their pid as their priority
    else if (getpid() > 15) 
    {
    priorityset(getpid(), currentprio);
    currentprio++;
    }
    // schedEnqueue();
    fork_chain(i-1);
    // wait();
    }

}

int main(void)
{
    //enque procs
    //int ppid = getpid();
    priorityset(getpid(), 1); //will not be given lower priority in currentprio
    testMode();
    fork_chain(15);
    while(wait()!=-1)
    {wait();}
    // if (getpid() == ppid)
    // {
    // printSched();
    // //start dequeuing 10 procs
    // // int i = 1;
    // // int dq = schedDequeue();
    // // printf(1,"Dequeue #%d: [pid: %d] \n", i, dq);
    // // printSched();
    // }
    testMode();
    exit();
}