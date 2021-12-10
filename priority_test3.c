// #include "types.h"
// #include "stat.h"
// #include "user.h"

// //static int currentprio = 0;

// void
// fork_chain(int i)
// {
//     int id;
//     if(i<0)
//     {return;}
//     id = fork();
//     if(id == 0)
//     {
//     int pid = getpid();
//     int prio = 1;
//         if(pid >= 10) 
//         {
//         prio = 15-pid;
//         priorityset(pid, prio); // procs with pid less than 10 get parent pid, procs with more get their pid as their priority
//         }
//     printf(1,"running proc #%d of priority [%d] \n", pid, prio);
//     fork_chain(i-1);
//     wait();
//     }

// }

// int main(void)
// {
//     //enque procs
//     int ppid = getpid();
//     priorityset(getpid(), 1);
//     fork_chain(10);
//     while(wait()!=-1)
//     {wait();}
//     if (getpid() == ppid)
//     {
//     printSched();
//     }
//     exit();
// }

#include "types.h"
#include "user.h"

int main(void) {
    //int ppid = getpid();
    int pid = fork();
    priorityset(getpid(), 1);
    testMode();
    if(pid > 0){
        for(int x = 0; x < 7; x++){
            pid = fork();
            if(getpid() >= 4) priorityset(getpid(), getpid());
            // sleep(100);
        }
    }
    // if(getpid() == ppid){
    //     pSchedArr();
    // }


    //if(pid != ppid) printf(1, "executing proc of priority %d \n", 11-getpid());
    while(wait() != -1);
    exit();
}