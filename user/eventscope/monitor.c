#include <stdio.h>
#include "intentkernel.h"

void eventscope_audit(const ipc_msg_t *msg) {
    if (!msg) {
        return;
    }
    printf("eventscope audit type %u from %llu\n", msg->type, (unsigned long long)msg->sender);
}
