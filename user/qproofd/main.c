#include <stdio.h>
#include <string.h>
#include "intentkernel.h"

int qproof_sign(const quantum_result_t *result, u8 *sig, size_t *sig_len);

int main(void) {
    ipc_init();
    ipc_channel_create(CH_QPROOFD, "qproofd");
    printf("qproofd listening on channel %d\n", CH_QPROOFD);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_QPROOFD, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type != MSG_QUANTUM_RESULT || msg.len < sizeof(quantum_result_t)) {
            continue;
        }

        quantum_result_t result;
        memcpy(&result, msg.data, sizeof(result));
        u8 sig[CAP_SIG_LEN];
        size_t sig_len = 0;
        if (qproof_sign(&result, sig, &sig_len) != 0) {
            continue;
        }

        ipc_msg_t reply;
        memset(&reply, 0, sizeof(reply));
        reply.type = MSG_PROOF_SIGN;
        reply.sender = CH_QPROOFD;
        reply.len = sizeof(result) + sig_len;
        memcpy(reply.data, &result, sizeof(result));
        memcpy(reply.data + sizeof(result), sig, sig_len);
        ipc_send(CH_EVENTSCOPE, &reply);
    }
    return 0;
}
