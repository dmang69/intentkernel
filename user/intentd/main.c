#include <stdio.h>
#include <string.h>
#include "intentkernel.h"

intent_id_t intentd_classify(const char *text, float *confidence);

int main(void) {
    ipc_init();
    ipc_channel_create(CH_INTENTD, "intentd");
    printf("intentd listening on channel %d\n", CH_INTENTD);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_INTENTD, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type != MSG_INTENT_CLASSIFY) {
            continue;
        }

        char text[257];
        size_t len = msg.len < sizeof(text) - 1 ? msg.len : sizeof(text) - 1;
        memcpy(text, msg.data, len);
        text[len] = '\0';

        float confidence = 0.0f;
        intent_id_t intent = intentd_classify(text, &confidence);

        ipc_msg_t reply;
        memset(&reply, 0, sizeof(reply));
        reply.type = MSG_INTENT_REPLY;
        reply.sender = CH_INTENTD;
        reply.len = sizeof(intent) + sizeof(confidence);
        memcpy(reply.data, &intent, sizeof(intent));
        memcpy(reply.data + sizeof(intent), &confidence, sizeof(confidence));
        ipc_send((int)msg.sender, &reply);
    }
    return 0;
}
