#include <string.h>
#include "intentkernel.h"

static pqc_ctx_t qproof_ctx;
static int qproof_initialized = 0;

static void qproof_init_once(void) {
    if (!qproof_initialized) {
        pqc_init(&qproof_ctx);
        qproof_initialized = 1;
    }
}

int qproof_sign(const quantum_result_t *result, u8 *sig, size_t *sig_len) {
    if (!result || !sig || !sig_len) {
        return -1;
    }
    qproof_init_once();
    return pqc_sign((const u8 *)result, sizeof(*result), &qproof_ctx, sig, sig_len);
}
