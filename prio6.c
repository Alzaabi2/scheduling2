#include "types.h"
#include "user.h"

int prio6(void) {
    sleep(50);
    priorityset(getpid(), 6);
    sleep(50);

    printf(1,"executing proc of priority 6 \n");
    return 0;
}