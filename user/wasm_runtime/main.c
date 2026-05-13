#include <stdio.h>
#include "intentkernel.h"

int wasm_execute(const u8 *bytes, size_t len);

int main(void) {
    ipc_init();
    ipc_channel_create(CH_WASM, "wasm_runtime");
    printf("wasm_runtime listening on channel %d\n", CH_WASM);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_WASM, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type != MSG_WASM_EXEC) {
            continue;
        }
        wasm_execute(msg.data, msg.len);
    }
    return 0;
}
