#ifndef INTENTKERNEL_PQC_H
#define INTENTKERNEL_PQC_H

#include <stddef.h>
#include "types.h"

typedef struct {
    u8 pk[1312];
    u8 sk[2528];
    u64 merkle_root;
} pqc_ctx_t;

void pqc_init(pqc_ctx_t *ctx);
int pqc_sign(const u8 *msg, size_t msg_len, const pqc_ctx_t *sk, u8 *sig, size_t *sig_len);
int pqc_verify(const u8 *msg, size_t msg_len, const pqc_ctx_t *pk, const u8 *sig, size_t sig_len);
u64 quantum_sentinel_merkle(pqc_ctx_t *keys, int n);
void sha3_256(const u8 *in, size_t inlen, u8 *out);

#endif
