from ikmos.core.security import request_capability


def submit(circuit: str) -> str:
    request_capability("CAP_QUANTUM_SUBMIT", circuit)
    return f"[ikmos] submitted quantum circuit: {circuit.strip() or '<empty>'}"
