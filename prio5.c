#include "types.h"
#include "user.h"

int prio5(void) {
    sleep(50);
    priorityset(getpid(), 5);
    sleep(50);

    printf(1,"executing proc of priority 5 \n");
    return 0;
}