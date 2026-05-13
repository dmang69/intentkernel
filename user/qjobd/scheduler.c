#include "intentkernel.h"

int qjobd_dispatch(const ipc_msg_t *msg) {
    if (!msg) {
        return -1;
    }
    return ipc_send(CH_QSIMD, msg);
}
