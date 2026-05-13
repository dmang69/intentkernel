#ifndef INTENTKERNEL_CAPS_H
#define INTENTKERNEL_CAPS_H

#include "types.h"

cap_token_t cap_issue(u32 cap_type, const char *context);
int cap_verify(cap_token_t *cap, u32 required_type);
int cap_revoke(cap_token_t *cap);
int cap_has_type(cap_token_t *cap, u32 type);
u64 cap_expiry(cap_token_t *cap);

#endif
