#include <stdio.h>
#include <string.h>
#include "intentkernel.h"

int qsimd_run(quantum_result_t *result);

int main(void) {
    ipc_init();
    ipc_channel_create(CH_QSIMD, "qsimd");
    printf("qsimd listening on channel %d\n", CH_QSIMD);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_QSIMD, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type != MSG_JOB_ENQUEUE) {
            continue;
        }

        quantum_result_t result;
        if (qsimd_run(&result) != 0) {
            continue;
        }

        ipc_msg_t reply;
        memset(&reply, 0, sizeof(reply));
        reply.type = MSG_QUANTUM_RESULT;
        reply.sender = CH_QSIMD;
        reply.len = sizeof(result);
        memcpy(reply.data, &result, sizeof(result));
        ipc_send(CH_QPROOFD, &reply);
    }
    return 0;
}
