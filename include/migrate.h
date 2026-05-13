#ifndef INTENTKERNEL_MIGRATE_H
#define INTENTKERNEL_MIGRATE_H

#include <stddef.h>
#include "types.h"

#define MIGRATE_MAX_APPS 32
#define MIGRATE_MAX_SETTINGS 32
#define MIGRATE_MAX_GRANTS 64

typedef struct {
    char name[64];
    char category[32];
} migrate_app_t;

typedef struct {
    char key[64];
    char value[128];
} migrate_kv_t;

typedef struct {
    char app[64];
    u32 cap;
} migrate_cap_grant_t;

typedef struct {
    char user_data[128];
    migrate_app_t apps[MIGRATE_MAX_APPS];
    size_t app_count;
    migrate_kv_t settings[MIGRATE_MAX_SETTINGS];
    size_t setting_count;
    migrate_cap_grant_t grants[MIGRATE_MAX_GRANTS];
    size_t grant_count;
} migrate_manifest_t;

int migrate_manifest_parse(const char *buf, migrate_manifest_t *out);
int migrate_manifest_serialize(const migrate_manifest_t *manifest, char *out, size_t out_len);
int migrate_backup(const char *path, migrate_manifest_t *manifest);
int migrate_restore(const char *archive, const migrate_manifest_t *manifest);
int migrate_validate(const migrate_manifest_t *manifest);
size_t migrate_list_apps(const migrate_manifest_t *manifest, migrate_app_t *apps, size_t max_apps);
size_t migrate_list_caps(const migrate_manifest_t *manifest, const char *app_name, u32 *caps, size_t max_caps);

#endif
