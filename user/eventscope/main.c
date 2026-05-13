#include <stdio.h>
#include "intentkernel.h"

void eventscope_audit(const ipc_msg_t *msg);

int main(void) {
    ipc_init();
    ipc_channel_create(CH_EVENTSCOPE, "eventscope");
    printf("eventscope listening on channel %d\n", CH_EVENTSCOPE);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_EVENTSCOPE, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type == MSG_EVENT_AUDIT || msg.type == MSG_PROOF_SIGN) {
            eventscope_audit(&msg);
        }
    }
    return 0;
}
