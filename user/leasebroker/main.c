#include <stdio.h>
#include <string.h>
#include "intentkernel.h"

int lease_issue(const cap_token_t *parent, lease_t *out);

int main(void) {
    ipc_init();
    ipc_channel_create(CH_LEASEBROKER, "leasebroker");
    printf("leasebroker listening on channel %d\n", CH_LEASEBROKER);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_LEASEBROKER, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type != MSG_LEASE_REQUEST || msg.len < sizeof(cap_token_t)) {
            continue;
        }
        cap_token_t parent;
        memcpy(&parent, msg.data, sizeof(parent));

        lease_t lease;
        if (lease_issue(&parent, &lease) != 0) {
            continue;
        }

        ipc_msg_t reply;
        memset(&reply, 0, sizeof(reply));
        reply.type = MSG_LEASE_REPLY;
        reply.sender = CH_LEASEBROKER;
        reply.len = sizeof(lease);
        memcpy(reply.data, &lease, sizeof(lease));
        ipc_send((int)msg.sender, &reply);
    }
    return 0;
}
