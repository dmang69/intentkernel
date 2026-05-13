#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "quantum.h"

static qcomplex_t qcomplex_add(qcomplex_t a, qcomplex_t b) {
    qcomplex_t r = { a.real + b.real, a.imag + b.imag };
    return r;
}

static qcomplex_t qcomplex_sub(qcomplex_t a, qcomplex_t b) {
    qcomplex_t r = { a.real - b.real, a.imag - b.imag };
    return r;
}

static qcomplex_t qcomplex_mul(qcomplex_t a, qcomplex_t b) {
    qcomplex_t r = { a.real * b.real - a.imag * b.imag, a.real * b.imag + a.imag * b.real };
    return r;
}

int quantum_state_init(quantum_state_t *state, int qubits) {
    if (!state || qubits <= 0 || qubits > 8) {
        return -1;
    }
    size_t len = (size_t)1 << qubits;
    state->state = (qcomplex_t *)calloc(len, sizeof(qcomplex_t));
    if (!state->state) {
        return -1;
    }
    state->qubits = qubits;
    state->state_len = len;
    state->state[0].real = 1.0;
    return 0;
}

void quantum_state_free(quantum_state_t *state) {
    if (!state) {
        return;
    }
    free(state->state);
    state->state = NULL;
    state->state_len = 0;
    state->qubits = 0;
}

static void apply_single_qubit_gate(quantum_state_t *state, int target, qcomplex_t m00, qcomplex_t m01, qcomplex_t m10, qcomplex_t m11) {
    size_t size = state->state_len;
    size_t mask = (size_t)1 << target;
    for (size_t i = 0; i < size; i++) {
        if ((i & mask) == 0) {
            size_t j = i | mask;
            qcomplex_t a = state->state[i];
            qcomplex_t b = state->state[j];
            state->state[i] = qcomplex_add(qcomplex_mul(m00, a), qcomplex_mul(m01, b));
            state->state[j] = qcomplex_add(qcomplex_mul(m10, a), qcomplex_mul(m11, b));
        }
    }
}

static void apply_cnot(quantum_state_t *state, int control, int target) {
    size_t size = state->state_len;
    size_t control_mask = (size_t)1 << control;
    size_t target_mask = (size_t)1 << target;
    for (size_t i = 0; i < size; i++) {
        if ((i & control_mask) && ((i & target_mask) == 0)) {
            size_t j = i | target_mask;
            qcomplex_t tmp = state->state[i];
            state->state[i] = state->state[j];
            state->state[j] = tmp;
        }
    }
}

int quantum_apply_gate(quantum_state_t *state, quantum_op_t op) {
    if (!state || !state->state) {
        return -1;
    }
    double inv_sqrt2 = 1.0 / sqrt(2.0);
    qcomplex_t one = {1.0, 0.0};
    qcomplex_t zero = {0.0, 0.0};

    switch (op.gate) {
    case QGATE_H:
        apply_single_qubit_gate(state, op.target,
            (qcomplex_t){inv_sqrt2, 0.0}, (qcomplex_t){inv_sqrt2, 0.0},
            (qcomplex_t){inv_sqrt2, 0.0}, (qcomplex_t){-inv_sqrt2, 0.0});
        break;
    case QGATE_X:
        apply_single_qubit_gate(state, op.target, zero, one, one, zero);
        break;
    case QGATE_Y:
        apply_single_qubit_gate(state, op.target,
            zero, (qcomplex_t){0.0, -1.0}, (qcomplex_t){0.0, 1.0}, zero);
        break;
    case QGATE_Z:
        apply_single_qubit_gate(state, op.target, one, zero, zero, (qcomplex_t){-1.0, 0.0});
        break;
    case QGATE_CNOT:
        apply_cnot(state, op.control, op.target);
        break;
    case QGATE_RX: {
        double c = cos(op.theta / 2.0);
        double s = sin(op.theta / 2.0);
        apply_single_qubit_gate(state, op.target,
            (qcomplex_t){c, 0.0}, (qcomplex_t){0.0, -s},
            (qcomplex_t){0.0, -s}, (qcomplex_t){c, 0.0});
        break; }
    case QGATE_RY: {
        double c = cos(op.theta / 2.0);
        double s = sin(op.theta / 2.0);
        apply_single_qubit_gate(state, op.target,
            (qcomplex_t){c, 0.0}, (qcomplex_t){-s, 0.0},
            (qcomplex_t){s, 0.0}, (qcomplex_t){c, 0.0});
        break; }
    case QGATE_RZ: {
        double c = cos(op.theta / 2.0);
        double s = sin(op.theta / 2.0);
        apply_single_qubit_gate(state, op.target,
            (qcomplex_t){c, -s}, zero, zero, (qcomplex_t){c, s});
        break; }
    case QGATE_MEASURE:
        break;
    default:
        return -1;
    }
    return 0;
}

int quantum_measure_all(const quantum_state_t *state, quantum_result_t *result) {
    if (!state || !state->state || !result) {
        return -1;
    }
    memset(result, 0, sizeof(*result));
    result->qubits = state->qubits;
    for (size_t i = 0; i < state->state_len; i++) {
        double prob = state->state[i].real * state->state[i].real + state->state[i].imag * state->state[i].imag;
        result->counts[i] = (u64)(prob * 10000.0);
    }
    return 0;
}

int quantum_run_circuit(const quantum_circuit_t *circuit, quantum_result_t *result) {
    if (!circuit || !result) {
        return -1;
    }
    quantum_state_t state;
    if (quantum_state_init(&state, result->qubits) != 0) {
        return -1;
    }
    for (size_t i = 0; i < circuit->count; i++) {
        quantum_apply_gate(&state, circuit->ops[i]);
    }
    int rc = quantum_measure_all(&state, result);
    quantum_state_free(&state);
    return rc;
}
