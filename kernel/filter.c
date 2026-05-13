#include "intentkernel.h"

int filter_validate(intent_id_t intent, cap_token_t *cap) {
    if (!cap) {
        return -1;
    }

    u32 required = 0;
    switch (intent) {
    case INTENT_PRINT:
        required = CAP_PRINT;
        break;
    case INTENT_FILE_READ:
        required = CAP_FILE_READ;
        break;
    case INTENT_FILE_WRITE:
        required = CAP_FILE_WRITE;
        break;
    case INTENT_NETWORK_CONNECT:
        required = CAP_NETWORK;
        break;
    case INTENT_PROCESS_SPAWN:
        required = CAP_PROCESS;
        break;
    case INTENT_WASM_EXEC:
        required = CAP_WASM_EXEC;
        break;
    case INTENT_QUANTUM_SUBMIT:
    case INTENT_QUANTUM_SIM:
        required = CAP_QUANTUM_SUBMIT;
        break;
    case INTENT_ROOT:
        required = CAP_ROOT;
        break;
    default:
        required = 0;
        break;
    }

    return cap_verify(cap, required);
}
