#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intentkernel.h"

static const char *default_archive(void) {
    const char *env = getenv("INTENTKERNEL_MIGRATE_ARCHIVE");
    return env && env[0] ? env : "migration.manifest";
}

static int load_manifest(const char *path, migrate_manifest_t *manifest) {
    if (!path || !manifest) {
        return -1;
    }
    FILE *file = fopen(path, "r");
    if (!file) {
        return -1;
    }
    char buffer[4096];
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[len] = '\0';
    fclose(file);
    return migrate_manifest_parse(buffer, manifest);
}

static void print_usage(void) {
    printf("Usage:\n");
    printf("  intentkernel-migrate backup [path]\n");
    printf("  intentkernel-migrate restore <archive>\n");
    printf("  intentkernel-migrate validate <manifest>\n");
    printf("  intentkernel-migrate list-apps\n");
    printf("  intentkernel-migrate list-caps <app>\n");
    printf("  intentkernel-migrate status\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    const char *cmd = argv[1];
    if (strcmp(cmd, "backup") == 0) {
        const char *path = argc > 2 ? argv[2] : default_archive();
        migrate_manifest_t manifest;
        if (migrate_backup(path, &manifest) != 0) {
            fprintf(stderr, "Backup failed\n");
            return 1;
        }
        printf("Backup written to %s\n", path);
        return 0;
    }

    if (strcmp(cmd, "restore") == 0) {
        if (argc < 3) {
            print_usage();
            return 1;
        }
        if (migrate_restore(argv[2], NULL) != 0) {
            fprintf(stderr, "Restore failed\n");
            return 1;
        }
        printf("Restore completed\n");
        return 0;
    }

    if (strcmp(cmd, "validate") == 0) {
        if (argc < 3) {
            print_usage();
            return 1;
        }
        migrate_manifest_t manifest;
        if (load_manifest(argv[2], &manifest) != 0) {
            fprintf(stderr, "Failed to load manifest\n");
            return 1;
        }
        if (migrate_validate(&manifest) != 0) {
            fprintf(stderr, "Manifest invalid\n");
            return 1;
        }
        printf("Manifest valid\n");
        return 0;
    }

    if (strcmp(cmd, "list-apps") == 0) {
        migrate_manifest_t manifest;
        const char *path = default_archive();
        if (load_manifest(path, &manifest) != 0) {
            fprintf(stderr, "Failed to load manifest %s\n", path);
            return 1;
        }
        for (size_t i = 0; i < manifest.app_count; i++) {
            printf("%s (%s)\n", manifest.apps[i].name, manifest.apps[i].category);
        }
        return 0;
    }

    if (strcmp(cmd, "list-caps") == 0) {
        if (argc < 3) {
            print_usage();
            return 1;
        }
        migrate_manifest_t manifest;
        const char *path = default_archive();
        if (load_manifest(path, &manifest) != 0) {
            fprintf(stderr, "Failed to load manifest %s\n", path);
            return 1;
        }
        u32 caps[32];
        size_t count = migrate_list_caps(&manifest, argv[2], caps, 32);
        for (size_t i = 0; i < count; i++) {
            printf("0x%X\n", caps[i]);
        }
        return 0;
    }

    if (strcmp(cmd, "status") == 0) {
        const char *path = default_archive();
        FILE *file = fopen(path, "r");
        if (file) {
            fclose(file);
            printf("Archive found: %s\n", path);
        } else {
            printf("No archive found at %s\n", path);
        }
        return 0;
    }

    print_usage();
    return 1;
}
