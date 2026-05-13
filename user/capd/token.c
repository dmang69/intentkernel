#include "intentkernel.h"

cap_token_t capd_issue(u32 type, const char *context) {
    return cap_issue(type, context);
}

int capd_revoke(cap_token_t *cap) {
    return cap_revoke(cap);
}
