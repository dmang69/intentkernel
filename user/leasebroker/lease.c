#include <string.h>
#include <time.h>
#include "intentkernel.h"

static pqc_ctx_t lease_ctx;
static int lease_initialized = 0;

static void lease_init_once(void) {
    if (!lease_initialized) {
        pqc_init(&lease_ctx);
        lease_initialized = 1;
    }
}

int lease_issue(const cap_token_t *parent, lease_t *out) {
    if (!parent || !out) {
        return -1;
    }
    lease_init_once();

    if (cap_verify((cap_token_t *)parent, parent->type) != 0) {
        return -1;
    }

    memset(out, 0, sizeof(*out));
    out->parent = *parent;
    out->expiry = (u64)time(NULL) + 60;

    size_t sig_len = 0;
    pqc_sign((const u8 *)out, sizeof(*out) - CAP_SIG_LEN, &lease_ctx, out->sig, &sig_len);
    return 0;
}
