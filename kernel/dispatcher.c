#include "intentkernel.h"

int filter_validate(intent_id_t intent, cap_token_t *cap);

int dispatch_intent(intent_id_t intent, cap_token_t *cap) {
    if (filter_validate(intent, cap) != 0) {
        return -1;
    }
    return 0;
}
