#ifndef INTENTKERNEL_MANIFEST_H
#define INTENTKERNEL_MANIFEST_H

#include <stddef.h>
#include "types.h"

#define MANIFEST_MAX_CAPS 32

typedef struct {
    char app_name[64];
    char category[32];
    char system_version[32];
    u32 caps[MANIFEST_MAX_CAPS];
    size_t cap_count;
} app_manifest_t;

int manifest_parse(const char *buf, app_manifest_t *out);
int manifest_validate(const app_manifest_t *mf, const char *system_version);
size_t manifest_to_caps(const app_manifest_t *mf, cap_token_t *out_caps, size_t max_caps, const char *context);

#endif
