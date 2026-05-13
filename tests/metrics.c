#include <stdio.h>
#include <string.h>
#include "metrics.h"

void metrics_init(metrics_t *metrics) {
    if (!metrics) {
        return;
    }
    memset(metrics, 0, sizeof(*metrics));
}

void metrics_record_denial(metrics_t *metrics) {
    if (metrics) {
        metrics->denials++;
    }
}

void metrics_record_escape(metrics_t *metrics) {
    if (metrics) {
        metrics->escapes++;
    }
}

void metrics_write_csv(const metrics_t *metrics, const char *path) {
    if (!metrics || !path) {
        return;
    }
    FILE *file = fopen(path, "w");
    if (!file) {
        return;
    }
    fprintf(file, "denials,escapes,token_forgery,syscall_abuse,wasm_escape,ipc_attack,quantum_tamper,cap_issue_time_ms,ipc_latency_ms,intent_accuracy\n");
    fprintf(file, "%u,%u,%u,%u,%u,%u,%u,%.2f,%.2f,%.2f\n",
        metrics->denials,
        metrics->escapes,
        metrics->token_forgery,
        metrics->syscall_abuse,
        metrics->wasm_escape,
        metrics->ipc_attack,
        metrics->quantum_tamper,
        metrics->cap_issue_time_ms,
        metrics->ipc_latency_ms,
        metrics->intent_accuracy);
    fclose(file);
}
