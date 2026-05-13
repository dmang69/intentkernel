#include <stdio.h>
#include <string.h>
#include "intentkernel.h"

static int test_app_manifest(void) {
    const char *buf =
        "app=demo_app\n"
        "category=system\n"
        "system_version=0.3\n"
        "cap=CAP_FILE_READ\n"
        "cap=CAP_NETWORK\n";
    app_manifest_t mf;
    if (manifest_parse(buf, &mf) != 0) {
        printf("manifest_parse failed\n");
        return -1;
    }
    if (manifest_validate(&mf, "0.3") != 0) {
        printf("manifest_validate failed\n");
        return -1;
    }
    cap_token_t caps[MANIFEST_MAX_CAPS];
    size_t count = manifest_to_caps(&mf, caps, MANIFEST_MAX_CAPS, mf.app_name);
    if (count != 2) {
        printf("manifest_to_caps count mismatch\n");
        return -1;
    }
    if (caps[0].flags != CAP_FLAG_MIGRATION_PERSIST) {
        printf("caps missing migration flag\n");
        return -1;
    }
    return 0;
}

static int test_migration_manifest(void) {
    const char *buf =
        "user_data=/home/demo\n"
        "app=demo_app\n"
        "category=system\n"
        "setting=region=us\n"
        "grant=demo_app:CAP_FILE_READ\n";
    migrate_manifest_t manifest;
    if (migrate_manifest_parse(buf, &manifest) != 0) {
        printf("migrate_manifest_parse failed\n");
        return -1;
    }
    if (migrate_validate(&manifest) != 0) {
        printf("migrate_validate failed\n");
        return -1;
    }
    u32 caps[4];
    size_t count = migrate_list_caps(&manifest, "demo_app", caps, 4);
    if (count != 1 || caps[0] != CAP_FILE_READ) {
        printf("migrate_list_caps failed\n");
        return -1;
    }
    return 0;
}

static int test_policy(void) {
    if (policy_validate_app("bloatware") == 0) {
        printf("policy_validate_app failed\n");
        return -1;
    }
    if (policy_validate_app("system") != 0) {
        printf("policy_validate_app false positive\n");
        return -1;
    }
    return 0;
}

int main(void) {
    if (test_app_manifest() != 0) {
        return 1;
    }
    if (test_migration_manifest() != 0) {
        return 1;
    }
    if (test_policy() != 0) {
        return 1;
    }
    printf("Migration tests passed\n");
    return 0;
}
