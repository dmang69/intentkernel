#include <stdio.h>
#include <string.h>
#include "intentkernel.h"

int quantum_validate_cap(const cap_token_t *cap);

int main(void) {
    ipc_init();
    ipc_channel_create(CH_QUANTUMD, "quantumd");
    printf("quantumd listening on channel %d\n", CH_QUANTUMD);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_QUANTUMD, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type != MSG_QUANTUM_SUBMIT || msg.len < sizeof(cap_token_t)) {
            continue;
        }
        cap_token_t cap;
        memcpy(&cap, msg.data, sizeof(cap));
        if (quantum_validate_cap(&cap) != 0) {
            continue;
        }
        msg.sender = CH_QUANTUMD;
        ipc_send(CH_QJOBD, &msg);
    }
    return 0;
}
