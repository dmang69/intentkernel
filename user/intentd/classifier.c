#include <string.h>
#include "intentkernel.h"

intent_id_t intentd_classify(const char *text, float *confidence) {
    intent_id_t intent = INTENT_ROOT;
    float conf = 0.0f;
    if (ml_pattern_match(text, &intent, &conf)) {
        if (confidence) {
            *confidence = conf;
        }
        return intent;
    }

    float features[32];
    memset(features, 0, sizeof(features));
    if (text) {
        size_t len = strlen(text);
        for (size_t i = 0; i < len; i++) {
            features[i % 32] += 0.1f;
        }
    }
    ml_result_t result = ml_classify(features);
    if (confidence) {
        *confidence = result.confidence;
    }
    return (intent_id_t)(result.class_id + 1);
}
