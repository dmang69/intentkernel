#ifndef INTENTKERNEL_QUANTUM_H
#define INTENTKERNEL_QUANTUM_H

#include <stddef.h>
#include "types.h"

typedef struct {
    double real;
    double imag;
} qcomplex_t;

typedef enum {
    QGATE_H,
    QGATE_X,
    QGATE_Y,
    QGATE_Z,
    QGATE_CNOT,
    QGATE_RX,
    QGATE_RY,
    QGATE_RZ,
    QGATE_MEASURE
} quantum_gate_t;

typedef struct {
    quantum_gate_t gate;
    int target;
    int control;
    double theta;
} quantum_op_t;

typedef struct {
    int qubits;
    size_t state_len;
    qcomplex_t *state;
} quantum_state_t;

typedef struct {
    quantum_op_t *ops;
    size_t count;
} quantum_circuit_t;

typedef struct {
    int qubits;
    u64 counts[16];
} quantum_result_t;

int quantum_state_init(quantum_state_t *state, int qubits);
void quantum_state_free(quantum_state_t *state);
int quantum_apply_gate(quantum_state_t *state, quantum_op_t op);
int quantum_measure_all(const quantum_state_t *state, quantum_result_t *result);
int quantum_run_circuit(const quantum_circuit_t *circuit, quantum_result_t *result);

#endif
