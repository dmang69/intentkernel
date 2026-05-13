#include <stdio.h>
#include "intentkernel.h"

int qjobd_dispatch(const ipc_msg_t *msg);

int main(void) {
    ipc_init();
    ipc_channel_create(CH_QJOBD, "qjobd");
    printf("qjobd listening on channel %d\n", CH_QJOBD);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_QJOBD, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type != MSG_JOB_ENQUEUE) {
            continue;
        }
        qjobd_dispatch(&msg);
    }
    return 0;
}
