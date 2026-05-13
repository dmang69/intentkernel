#include "intentkernel.h"

int quantum_validate_cap(const cap_token_t *cap) {
    if (!cap) {
        return -1;
    }
    return cap_verify((cap_token_t *)cap, CAP_QUANTUM_SUBMIT);
}
