#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ml.h"

static float weights1[16][32];
static float weights2[10][16];
static float bias1[16];
static float bias2[10];
static int ml_initialized = 0;

static float ml_rand_weight(int seed) {
    return (float)(sin(seed * 12.9898) * 43758.5453);
}

void ml_init(void) {
    if (ml_initialized) {
        return;
    }
    for (int i = 0; i < 16; i++) {
        bias1[i] = ml_rand_weight(i) * 0.01f;
        for (int j = 0; j < 32; j++) {
            weights1[i][j] = ml_rand_weight(i * 32 + j) * 0.001f;
        }
    }
    for (int i = 0; i < 10; i++) {
        bias2[i] = ml_rand_weight(500 + i) * 0.01f;
        for (int j = 0; j < 16; j++) {
            weights2[i][j] = ml_rand_weight(600 + i * 16 + j) * 0.001f;
        }
    }
    ml_initialized = 1;
}

static float relu(float x) {
    return x > 0.0f ? x : 0.0f;
}

ml_result_t ml_classify(const float features[32]) {
    ml_init();
    float hidden[16];
    for (int i = 0; i < 16; i++) {
        float sum = bias1[i];
        for (int j = 0; j < 32; j++) {
            sum += weights1[i][j] * features[j];
        }
        hidden[i] = relu(sum);
    }

    float logits[10];
    float max_logit = -1e9f;
    for (int i = 0; i < 10; i++) {
        float sum = bias2[i];
        for (int j = 0; j < 16; j++) {
            sum += weights2[i][j] * hidden[j];
        }
        logits[i] = sum;
        if (sum > max_logit) {
            max_logit = sum;
        }
    }

    float exp_sum = 0.0f;
    for (int i = 0; i < 10; i++) {
        logits[i] = expf(logits[i] - max_logit);
        exp_sum += logits[i];
    }

    ml_result_t result = {0, 0.0f};
    for (int i = 0; i < 10; i++) {
        float prob = logits[i] / exp_sum;
        if (prob > result.confidence) {
            result.confidence = prob;
            result.class_id = i;
        }
    }
    return result;
}

int ml_pattern_match(const char *text, intent_id_t *intent_out, float *confidence_out) {
    if (!text || !intent_out || !confidence_out) {
        return 0;
    }
    if (strstr(text, "read")) {
        *intent_out = INTENT_FILE_READ;
        *confidence_out = 0.95f;
        return 1;
    }
    if (strstr(text, "write")) {
        *intent_out = INTENT_FILE_WRITE;
        *confidence_out = 0.95f;
        return 1;
    }
    if (strstr(text, "network") || strstr(text, "http")) {
        *intent_out = INTENT_NETWORK_CONNECT;
        *confidence_out = 0.9f;
        return 1;
    }
    if (strstr(text, "quantum")) {
        *intent_out = INTENT_QUANTUM_SUBMIT;
        *confidence_out = 0.9f;
        return 1;
    }
    if (strstr(text, "print")) {
        *intent_out = INTENT_PRINT;
        *confidence_out = 0.9f;
        return 1;
    }
    return 0;
}
