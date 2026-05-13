#include <stdio.h>
#include <string.h>
#include "intentkernel.h"

cap_token_t capd_issue(u32 type, const char *context);
int capd_revoke(cap_token_t *cap);

static void handle_issue(const ipc_msg_t *msg) {
    if (!msg) {
        return;
    }
    u32 cap_type = 0;
    memcpy(&cap_type, msg->data, sizeof(cap_type));
    const char *context = (const char *)(msg->data + sizeof(cap_type));

    cap_token_t token = capd_issue(cap_type, context);

    ipc_msg_t reply;
    memset(&reply, 0, sizeof(reply));
    reply.type = MSG_TOKEN_REPLY;
    reply.sender = CH_CAPD;
    reply.len = sizeof(token);
    memcpy(reply.data, &token, sizeof(token));
    ipc_send((int)msg->sender, &reply);
}

static void handle_revoke(const ipc_msg_t *msg) {
    if (!msg || msg->len < sizeof(cap_token_t)) {
        return;
    }
    cap_token_t token;
    memcpy(&token, msg->data, sizeof(token));
    capd_revoke(&token);
}

int main(void) {
    ipc_init();
    ipc_channel_create(CH_CAPD, "capd");
    printf("capd listening on channel %d\n", CH_CAPD);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_CAPD, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type == MSG_TOKEN_ISSUE) {
            handle_issue(&msg);
        } else if (msg.type == MSG_TOKEN_REVOKE) {
            handle_revoke(&msg);
        }
    }
    return 0;
}
