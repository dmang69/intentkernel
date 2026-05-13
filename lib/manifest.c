#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "manifest.h"
#include "caps.h"

static u32 manifest_cap_from_string(const char *value) {
    if (!value) {
        return 0;
    }
    if (strcasecmp(value, "CAP_PRINT") == 0) return CAP_PRINT;
    if (strcasecmp(value, "CAP_FILE_READ") == 0) return CAP_FILE_READ;
    if (strcasecmp(value, "CAP_FILE_WRITE") == 0) return CAP_FILE_WRITE;
    if (strcasecmp(value, "CAP_NETWORK") == 0) return CAP_NETWORK;
    if (strcasecmp(value, "CAP_PROCESS") == 0) return CAP_PROCESS;
    if (strcasecmp(value, "CAP_WASM_EXEC") == 0) return CAP_WASM_EXEC;
    if (strcasecmp(value, "CAP_QUANTUM_SUBMIT") == 0) return CAP_QUANTUM_SUBMIT;
    if (strcasecmp(value, "CAP_ROOT") == 0) return CAP_ROOT;
    return (u32)strtoul(value, NULL, 0);
}

static char *trim(char *str) {
    while (str && isspace((unsigned char)*str)) {
        str++;
    }
    if (!str || *str == '\0') {
        return str;
    }
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        *end-- = '\0';
    }
    return str;
}

int manifest_parse(const char *buf, app_manifest_t *out) {
    if (!buf || !out) {
        return -1;
    }
    memset(out, 0, sizeof(*out));
    strncpy(out->category, "unknown", sizeof(out->category) - 1);

    char *copy = strdup(buf);
    if (!copy) {
        return -1;
    }
    char *saveptr = NULL;
    for (char *line = strtok_r(copy, "\n", &saveptr); line; line = strtok_r(NULL, "\n", &saveptr)) {
        char *trimmed = trim(line);
        if (!trimmed || trimmed[0] == '\0' || trimmed[0] == '#') {
            continue;
        }
        char *eq = strchr(trimmed, '=');
        if (!eq) {
            continue;
        }
        *eq = '\0';
        char *key = trim(trimmed);
        char *value = trim(eq + 1);
        if (strcasecmp(key, "app") == 0) {
            strncpy(out->app_name, value, sizeof(out->app_name) - 1);
        } else if (strcasecmp(key, "category") == 0) {
            strncpy(out->category, value, sizeof(out->category) - 1);
        } else if (strcasecmp(key, "system_version") == 0) {
            strncpy(out->system_version, value, sizeof(out->system_version) - 1);
        } else if (strcasecmp(key, "cap") == 0) {
            if (out->cap_count < MANIFEST_MAX_CAPS) {
                out->caps[out->cap_count++] = manifest_cap_from_string(value);
            }
        }
    }
    free(copy);
    return out->app_name[0] ? 0 : -1;
}

int manifest_validate(const app_manifest_t *mf, const char *system_version) {
    if (!mf) {
        return -1;
    }
    if (system_version && mf->system_version[0] && strcmp(mf->system_version, system_version) != 0) {
        return -1;
    }
    return mf->cap_count > 0 ? 0 : -1;
}

size_t manifest_to_caps(const app_manifest_t *mf, cap_token_t *out_caps, size_t max_caps, const char *context) {
    if (!mf || !out_caps || max_caps == 0) {
        return 0;
    }
    size_t count = 0;
    const char *ctx = context ? context : mf->app_name;
    for (size_t i = 0; i < mf->cap_count && count < max_caps; i++) {
        out_caps[count++] = cap_issue_with_flags(mf->caps[i], ctx, CAP_FLAG_MIGRATION_PERSIST);
    }
    return count;
}
