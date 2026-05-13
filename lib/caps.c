#include <stdio.h>
#include <string.h>
#include <time.h>
#include "caps.h"
#include "pqc.h"

#define CAP_TABLE_SIZE 256
#define CAP_DEFAULT_TTL 60

typedef struct {
    cap_token_t token;
    int revoked;
} cap_entry_t;

static cap_entry_t cap_table[CAP_TABLE_SIZE];
static u64 cap_slot_counter = 1;
static pqc_ctx_t cap_ctx;
static int cap_initialized = 0;

static void cap_init_once(void) {
    if (!cap_initialized) {
        pqc_init(&cap_ctx);
        cap_initialized = 1;
    }
}

static size_t cap_signing_payload(const cap_token_t *cap, const char *context, u8 *out, size_t out_len) {
    if (!cap || !out) {
        return 0;
    }
    size_t needed = sizeof(cap->slot) + sizeof(cap->expiry) + sizeof(cap->type);
    size_t ctx_len = context ? strlen(context) : 0;
    if (out_len < needed + ctx_len) {
        return 0;
    }
    size_t offset = 0;
    memcpy(out + offset, &cap->slot, sizeof(cap->slot));
    offset += sizeof(cap->slot);
    memcpy(out + offset, &cap->expiry, sizeof(cap->expiry));
    offset += sizeof(cap->expiry);
    memcpy(out + offset, &cap->type, sizeof(cap->type));
    offset += sizeof(cap->type);
    if (context && ctx_len) {
        memcpy(out + offset, context, ctx_len);
        offset += ctx_len;
    }
    return offset;
}

cap_token_t cap_issue(u32 cap_type, const char *context) {
    cap_init_once();

    cap_token_t cap;
    memset(&cap, 0, sizeof(cap));
    cap.slot = cap_slot_counter++;
    cap.type = cap_type;
    cap.expiry = (u64)time(NULL) + CAP_DEFAULT_TTL;

    u8 payload[256];
    size_t payload_len = cap_signing_payload(&cap, context, payload, sizeof(payload));
    size_t sig_len = 0;
    pqc_sign(payload, payload_len, &cap_ctx, cap.sig, &sig_len);

    size_t idx = cap.slot % CAP_TABLE_SIZE;
    cap_table[idx].token = cap;
    cap_table[idx].revoked = 0;
    return cap;
}

int cap_verify(cap_token_t *cap, u32 required_type) {
    if (!cap) {
        return -1;
    }
    cap_init_once();

    if (cap->expiry < (u64)time(NULL)) {
        return -1;
    }
    if (required_type != 0 && (cap->type & required_type) != required_type) {
        return -1;
    }

    size_t idx = cap->slot % CAP_TABLE_SIZE;
    if (cap_table[idx].revoked || cap_table[idx].token.slot != cap->slot) {
        return -1;
    }

    u8 payload[256];
    size_t payload_len = cap_signing_payload(cap, NULL, payload, sizeof(payload));
    return pqc_verify(payload, payload_len, &cap_ctx, cap->sig, CAP_SIG_LEN);
}

int cap_revoke(cap_token_t *cap) {
    if (!cap) {
        return -1;
    }
    size_t idx = cap->slot % CAP_TABLE_SIZE;
    cap_table[idx].revoked = 1;
    cap_table[idx].token.expiry = 0;
    return 0;
}

int cap_has_type(cap_token_t *cap, u32 type) {
    if (!cap) {
        return 0;
    }
    return ((cap->type & type) == type) ? 1 : 0;
}

u64 cap_expiry(cap_token_t *cap) {
    if (!cap) {
        return 0;
    }
    return cap->expiry;
}
