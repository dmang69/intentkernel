#include <stdio.h>
#include <string.h>
#include "intentkernel.h"

int ai_classify_risk(const char *text);

int main(void) {
    ipc_init();
    ipc_channel_create(CH_AI_CORE, "ai_core");
    printf("ai_core listening on channel %d\n", CH_AI_CORE);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_AI_CORE, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type != MSG_AI_CLASSIFY) {
            continue;
        }
        char text[257];
        size_t len = msg.len < sizeof(text) - 1 ? msg.len : sizeof(text) - 1;
        memcpy(text, msg.data, len);
        text[len] = '\0';

        int risk = ai_classify_risk(text);
        ipc_msg_t reply;
        memset(&reply, 0, sizeof(reply));
        reply.type = MSG_AI_CLASSIFY;
        reply.sender = CH_AI_CORE;
        reply.len = sizeof(risk);
        memcpy(reply.data, &risk, sizeof(risk));
        ipc_send((int)msg.sender, &reply);
    }
    return 0;
}
