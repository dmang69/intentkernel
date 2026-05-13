#include <stdio.h>
#include <string.h>
#include "intentkernel.h"

static migrate_manifest_t cached_manifest;
static int has_manifest = 0;

static void send_audit(const char *message) {
    if (!message) {
        return;
    }
    ipc_msg_t audit;
    memset(&audit, 0, sizeof(audit));
    audit.type = MSG_EVENT_AUDIT;
    audit.sender = CH_MIGRATED;
    audit.len = (u32)strlen(message);
    if (audit.len > sizeof(audit.data)) {
        audit.len = sizeof(audit.data);
    }
    memcpy(audit.data, message, audit.len);
    ipc_send(CH_EVENTSCOPE, &audit);
}

static void reissue_caps_via_capd(const migrate_manifest_t *manifest) {
    if (!manifest) {
        return;
    }
    for (size_t i = 0; i < manifest->grant_count; i++) {
        ipc_msg_t req;
        memset(&req, 0, sizeof(req));
        req.type = MSG_TOKEN_ISSUE;
        req.sender = CH_MIGRATED;
        u32 cap_type = manifest->grants[i].cap;
        memcpy(req.data, &cap_type, sizeof(cap_type));
        const char *context = manifest->grants[i].app;
        size_t context_len = strlen(context);
        if (context_len > sizeof(req.data) - sizeof(cap_type)) {
            context_len = sizeof(req.data) - sizeof(cap_type);
        }
        memcpy(req.data + sizeof(cap_type), context, context_len);
        req.len = sizeof(cap_type) + (u32)context_len;
        ipc_send(CH_CAPD, &req);
    }
}

static int load_manifest_file(const char *path, migrate_manifest_t *manifest) {
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

static void parse_request(const char *text, char *action, size_t action_len, char *payload, size_t payload_len) {
    if (!action || !payload) {
        return;
    }
    action[0] = '\0';
    payload[0] = '\0';
    if (!text) {
        return;
    }
    const char *space = strchr(text, ' ');
    if (!space) {
        size_t act_len = strlen(text);
        if (act_len >= action_len) {
            act_len = action_len - 1;
        }
        memcpy(action, text, act_len);
        action[act_len] = '\0';
        return;
    }
    size_t act_len = (size_t)(space - text);
    if (act_len >= action_len) {
        act_len = action_len - 1;
    }
    memcpy(action, text, act_len);
    action[act_len] = '\0';
    strncpy(payload, space + 1, payload_len - 1);
    payload[payload_len - 1] = '\0';
}

static void handle_request(const ipc_msg_t *msg) {
    if (!msg) {
        return;
    }
    char request[257];
    size_t len = msg->len < sizeof(request) - 1 ? msg->len : sizeof(request) - 1;
    memcpy(request, msg->data, len);
    request[len] = '\0';

    char action[32];
    char payload[192];
    parse_request(request, action, sizeof(action), payload, sizeof(payload));

    char response[256];
    int status = 0;
    if (strcmp(action, "backup") == 0) {
        const char *path = payload[0] ? payload : "migration.manifest";
        status = migrate_backup(path, &cached_manifest);
        has_manifest = (status == 0);
        snprintf(response, sizeof(response), "backup %s: %s", path, status == 0 ? "ok" : "failed");
        send_audit("migration backup requested");
    } else if (strcmp(action, "restore") == 0) {
        const char *path = payload[0] ? payload : "migration.manifest";
        migrate_manifest_t manifest;
        status = load_manifest_file(path, &manifest);
        if (status == 0) {
            status = migrate_restore(path, &manifest);
        }
        if (status == 0) {
            cached_manifest = manifest;
            has_manifest = 1;
            reissue_caps_via_capd(&manifest);
            send_audit("migration restore requested");
        }
        snprintf(response, sizeof(response), "restore %s: %s", path, status == 0 ? "ok" : "failed");
    } else if (strcmp(action, "validate") == 0) {
        const char *path = payload[0] ? payload : "migration.manifest";
        migrate_manifest_t manifest;
        status = load_manifest_file(path, &manifest);
        if (status == 0) {
            status = migrate_validate(&manifest);
        }
        snprintf(response, sizeof(response), "validate %s: %s", path, status == 0 ? "ok" : "failed");
    } else if (strcmp(action, "list-apps") == 0) {
        if (!has_manifest) {
            snprintf(response, sizeof(response), "no manifest cached");
        } else {
            size_t offset = 0;
            for (size_t i = 0; i < cached_manifest.app_count && offset < sizeof(response) - 1; i++) {
                int written = snprintf(response + offset, sizeof(response) - offset, "%s (%s)%s",
                    cached_manifest.apps[i].name,
                    cached_manifest.apps[i].category,
                    (i + 1 == cached_manifest.app_count) ? "" : ", ");
                if (written < 0) {
                    break;
                }
                offset += (size_t)written;
            }
            if (cached_manifest.app_count == 0) {
                snprintf(response, sizeof(response), "no apps");
            }
        }
    } else if (strcmp(action, "list-caps") == 0) {
        if (!has_manifest) {
            snprintf(response, sizeof(response), "no manifest cached");
        } else if (!payload[0]) {
            snprintf(response, sizeof(response), "missing app name");
        } else {
            u32 caps[16];
            size_t count = migrate_list_caps(&cached_manifest, payload, caps, 16);
            if (count == 0) {
                snprintf(response, sizeof(response), "no caps for %s", payload);
            } else {
                size_t offset = 0;
                for (size_t i = 0; i < count && offset < sizeof(response) - 1; i++) {
                    int written = snprintf(response + offset, sizeof(response) - offset, "0x%X%s",
                        caps[i],
                        (i + 1 == count) ? "" : ", ");
                    if (written < 0) {
                        break;
                    }
                    offset += (size_t)written;
                }
            }
        }
    } else if (strcmp(action, "status") == 0) {
        snprintf(response, sizeof(response), "manifest cached: %s, apps: %zu",
            has_manifest ? "yes" : "no",
            has_manifest ? cached_manifest.app_count : 0);
    } else {
        snprintf(response, sizeof(response), "unknown action: %s", action[0] ? action : "<empty>");
    }

    ipc_msg_t reply;
    memset(&reply, 0, sizeof(reply));
    reply.type = MSG_MIGRATE_REPLY;
    reply.sender = CH_MIGRATED;
    reply.len = (u32)strlen(response);
    if (reply.len > sizeof(reply.data)) {
        reply.len = sizeof(reply.data);
    }
    memcpy(reply.data, response, reply.len);
    ipc_send((int)msg->sender, &reply);
}

int main(void) {
    ipc_init();
    ipc_channel_create(CH_MIGRATED, "migrated");
    printf("migrated listening on channel %d\n", CH_MIGRATED);

    for (;;) {
        ipc_msg_t msg;
        int rc = ipc_recv(CH_MIGRATED, &msg, 500);
        if (rc <= 0) {
            continue;
        }
        if (msg.type == MSG_MIGRATE_REQUEST) {
            handle_request(&msg);
        }
    }
    return 0;
}
