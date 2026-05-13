#ifndef INTENTKERNEL_ML_H
#define INTENTKERNEL_ML_H

#include "types.h"

typedef struct {
    int class_id;
    float confidence;
} ml_result_t;

void ml_init(void);
ml_result_t ml_classify(const float features[32]);
int ml_pattern_match(const char *text, intent_id_t *intent_out, float *confidence_out);

#endif
