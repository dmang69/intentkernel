#include <string.h>
#include "cbor.h"

int cbor_encode_ipc_msg(const ipc_msg_t *msg, u8 *out, size_t *out_len) {
    if (!msg || !out || !out_len) {
        return -1;
    }
    if (*out_len < sizeof(ipc_msg_t)) {
        return -1;
    }
    memcpy(out, msg, sizeof(ipc_msg_t));
    *out_len = sizeof(ipc_msg_t);
    return 0;
}

int cbor_decode_ipc_msg(const u8 *data, size_t data_len, ipc_msg_t *msg) {
    if (!data || !msg) {
        return -1;
    }
    if (data_len < sizeof(ipc_msg_t)) {
        return -1;
    }
    memcpy(msg, data, sizeof(ipc_msg_t));
    return 0;
}
