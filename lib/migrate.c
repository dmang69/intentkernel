#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "migrate.h"
#include "caps.h"
#include "policy.h"

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

static u32 migrate_cap_from_string(const char *value) {
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

int migrate_manifest_parse(const char *buf, migrate_manifest_t *out) {
    if (!buf || !out) {
        return -1;
    }
    memset(out, 0, sizeof(*out));

    char *copy = strdup(buf);
    if (!copy) {
        return -1;
    }
    int last_app_index = -1;
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
        if (strcasecmp(key, "user_data") == 0) {
            strncpy(out->user_data, value, sizeof(out->user_data) - 1);
        } else if (strcasecmp(key, "app") == 0) {
            if (out->app_count < MIGRATE_MAX_APPS) {
                migrate_app_t *app = &out->apps[out->app_count++];
                memset(app, 0, sizeof(*app));
                strncpy(app->name, value, sizeof(app->name) - 1);
                strncpy(app->category, "unknown", sizeof(app->category) - 1);
                last_app_index = (int)out->app_count - 1;
            }
        } else if (strcasecmp(key, "category") == 0) {
            if (last_app_index >= 0) {
                migrate_app_t *app = &out->apps[last_app_index];
                strncpy(app->category, value, sizeof(app->category) - 1);
            }
        } else if (strcasecmp(key, "setting") == 0) {
            if (out->setting_count < MIGRATE_MAX_SETTINGS) {
                migrate_kv_t *setting = &out->settings[out->setting_count++];
                memset(setting, 0, sizeof(*setting));
                char *sep = strchr(value, '=');
                if (sep) {
                    *sep = '\0';
                    strncpy(setting->key, trim(value), sizeof(setting->key) - 1);
                    strncpy(setting->value, trim(sep + 1), sizeof(setting->value) - 1);
                } else {
                    strncpy(setting->key, value, sizeof(setting->key) - 1);
                }
            }
        } else if (strcasecmp(key, "grant") == 0) {
            if (out->grant_count < MIGRATE_MAX_GRANTS) {
                migrate_cap_grant_t *grant = &out->grants[out->grant_count++];
                memset(grant, 0, sizeof(*grant));
                char *sep = strchr(value, ':');
                if (sep) {
                    *sep = '\0';
                    strncpy(grant->app, trim(value), sizeof(grant->app) - 1);
                    grant->cap = migrate_cap_from_string(trim(sep + 1));
                } else {
                    strncpy(grant->app, value, sizeof(grant->app) - 1);
                }
            }
        }
    }
    free(copy);
    return out->app_count > 0 ? 0 : -1;
}

static int append_line(char *out, size_t out_len, size_t *offset, const char *line) {
    size_t needed = strlen(line);
    if (*offset + needed >= out_len) {
        return -1;
    }
    memcpy(out + *offset, line, needed);
    *offset += needed;
    out[*offset] = '\0';
    return 0;
}

int migrate_manifest_serialize(const migrate_manifest_t *manifest, char *out, size_t out_len) {
    if (!manifest || !out || out_len == 0) {
        return -1;
    }
    size_t offset = 0;
    out[0] = '\0';
    char line[256];
    if (manifest->user_data[0]) {
        snprintf(line, sizeof(line), "user_data=%s\n", manifest->user_data);
        if (append_line(out, out_len, &offset, line) != 0) return -1;
    }
    for (size_t i = 0; i < manifest->app_count; i++) {
        snprintf(line, sizeof(line), "app=%s\n", manifest->apps[i].name);
        if (append_line(out, out_len, &offset, line) != 0) return -1;
        snprintf(line, sizeof(line), "category=%s\n", manifest->apps[i].category);
        if (append_line(out, out_len, &offset, line) != 0) return -1;
    }
    for (size_t i = 0; i < manifest->setting_count; i++) {
        snprintf(line, sizeof(line), "setting=%s=%s\n", manifest->settings[i].key, manifest->settings[i].value);
        if (append_line(out, out_len, &offset, line) != 0) return -1;
    }
    for (size_t i = 0; i < manifest->grant_count; i++) {
        snprintf(line, sizeof(line), "grant=%s:0x%X\n", manifest->grants[i].app, manifest->grants[i].cap);
        if (append_line(out, out_len, &offset, line) != 0) return -1;
    }
    return 0;
}

int migrate_backup(const char *path, migrate_manifest_t *manifest) {
    if (!path || !manifest) {
        return -1;
    }
    memset(manifest, 0, sizeof(*manifest));
    strncpy(manifest->user_data, path, sizeof(manifest->user_data) - 1);
    manifest->app_count = 1;
    strncpy(manifest->apps[0].name, "ikmos_shell", sizeof(manifest->apps[0].name) - 1);
    strncpy(manifest->apps[0].category, "system", sizeof(manifest->apps[0].category) - 1);
    manifest->setting_count = 1;
    strncpy(manifest->settings[0].key, "version", sizeof(manifest->settings[0].key) - 1);
    strncpy(manifest->settings[0].value, "0.4", sizeof(manifest->settings[0].value) - 1);
    manifest->grant_count = 1;
    strncpy(manifest->grants[0].app, "ikmos_shell", sizeof(manifest->grants[0].app) - 1);
    manifest->grants[0].cap = CAP_FILE_READ;

    char buffer[2048];
    if (migrate_manifest_serialize(manifest, buffer, sizeof(buffer)) != 0) {
        return -1;
    }
    FILE *file = fopen(path, "w");
    if (!file) {
        return -1;
    }
    fputs(buffer, file);
    fclose(file);
    return 0;
}

int migrate_validate(const migrate_manifest_t *manifest) {
    if (!manifest) {
        return -1;
    }
    for (size_t i = 0; i < manifest->app_count; i++) {
        if (policy_validate_app(manifest->apps[i].category) != 0) {
            return -1;
        }
    }
    for (size_t i = 0; i < manifest->grant_count; i++) {
        int found = 0;
        for (size_t j = 0; j < manifest->app_count; j++) {
            if (strcmp(manifest->grants[i].app, manifest->apps[j].name) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return -1;
        }
    }
    return manifest->app_count > 0 ? 0 : -1;
}

int migrate_restore(const char *archive, const migrate_manifest_t *manifest) {
    migrate_manifest_t local_manifest;
    const migrate_manifest_t *use_manifest = manifest;
    if (!use_manifest) {
        if (!archive) {
            return -1;
        }
        FILE *file = fopen(archive, "r");
        if (!file) {
            return -1;
        }
        char buffer[4096];
        size_t len = fread(buffer, 1, sizeof(buffer) - 1, file);
        buffer[len] = '\0';
        fclose(file);
        if (migrate_manifest_parse(buffer, &local_manifest) != 0) {
            return -1;
        }
        use_manifest = &local_manifest;
    }
    if (migrate_validate(use_manifest) != 0) {
        return -1;
    }
    for (size_t i = 0; i < use_manifest->grant_count; i++) {
        cap_issue_with_flags(use_manifest->grants[i].cap, use_manifest->grants[i].app, CAP_FLAG_MIGRATION_PERSIST);
    }
    return 0;
}

size_t migrate_list_apps(const migrate_manifest_t *manifest, migrate_app_t *apps, size_t max_apps) {
    if (!manifest || !apps || max_apps == 0) {
        return 0;
    }
    size_t count = manifest->app_count < max_apps ? manifest->app_count : max_apps;
    for (size_t i = 0; i < count; i++) {
        apps[i] = manifest->apps[i];
    }
    return count;
}

size_t migrate_list_caps(const migrate_manifest_t *manifest, const char *app_name, u32 *caps, size_t max_caps) {
    if (!manifest || !app_name || !caps || max_caps == 0) {
        return 0;
    }
    size_t count = 0;
    for (size_t i = 0; i < manifest->grant_count && count < max_caps; i++) {
        if (strcmp(manifest->grants[i].app, app_name) == 0) {
            caps[count++] = manifest->grants[i].cap;
        }
    }
    return count;
}
