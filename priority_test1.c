#include "types.h"
#include "stat.h"
#include "user.h"

//static int currentprio = 0;

void
fork_chain(int i)
{
    int id;
    if(i<0)
    {return;}
    //children with pid 7 or less get parent priority
    if(getpid() > 7) priorityset(getpid(), getpid());//forked children with pid 7 get set to pid as priority
    id = fork();
    if(id == 0)
    {fork_chain(i-1);
    wait();}

}

int main(void)
{
    int ppid = getpid();
    int ret = priorityset(getpid(), 3); //will not be given lower priority in currentprio
    printf(1,"prioret : %d \n", ret);
    fork_chain(13);
    // int p;
    // p = priority(0);
    //printf(0,"current proc priority: %d \n", p);
    // if(fork() == 0)
    // {
    //     if(priorityset(0, 9) == -1) printf(1,"prioerror child\n");
    //         if(fork() == 0)
    //         { 
    //             sleep(50);
    //             exit();
    //         } 
    //     exit();
    // }
    // if(fork() == 0)
    // {
    //     //priorityset(getpid(), 6);
    //     //int p2 = priority(getpid());
    //     //printf(0,"child proc priority: %d \n", p2);
    //     exit();
    // }
    if(getpid() == ppid) ps();
    wait();
    exit();
}