#ifndef INTENTKERNEL_CBOR_H
#define INTENTKERNEL_CBOR_H

#include <stddef.h>
#include "types.h"

int cbor_encode_ipc_msg(const ipc_msg_t *msg, u8 *out, size_t *out_len);
int cbor_decode_ipc_msg(const u8 *data, size_t data_len, ipc_msg_t *msg);

#endif
