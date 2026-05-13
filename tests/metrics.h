#ifndef INTENTKERNEL_TEST_METRICS_H
#define INTENTKERNEL_TEST_METRICS_H

#include "intentkernel.h"

typedef struct {
    u32 denials;
    u32 escapes;
    u32 token_forgery;
    u32 syscall_abuse;
    u32 wasm_escape;
    u32 ipc_attack;
    u32 quantum_tamper;
    double cap_issue_time_ms;
    double ipc_latency_ms;
    float intent_accuracy;
} metrics_t;

void metrics_init(metrics_t *metrics);
void metrics_record_denial(metrics_t *metrics);
void metrics_record_escape(metrics_t *metrics);
void metrics_write_csv(const metrics_t *metrics, const char *path);

#endif
