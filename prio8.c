#include "types.h"
#include "user.h"

int prio8(void) {
    sleep(50);
    priorityset(getpid(), 8);
    sleep(50);

    printf(1,"executing proc of priority 8 \n");
    return 0;
}