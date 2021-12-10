#include "types.h"
#include "user.h"

int prio7(void) {
    sleep(50);
    priorityset(getpid(), 7);
    sleep(50);

    printf(1,"executing proc of priority 7 \n");
    return 0;
}