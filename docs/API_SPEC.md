# IntentKernel v0.4 API Specification

This document summarizes the primary public APIs consumed by daemons and IK-MOS.

## IPC

- `ipc_init()` initializes `/tmp/intentkernel` and channel state.
- `ipc_channel_create()` binds a Unix domain datagram socket for a channel.
- `ipc_send()` sends a serialized `ipc_msg_t`.
- `ipc_recv()` receives an `ipc_msg_t` with optional timeout.

## Capabilities

- `cap_issue()` issues a new `cap_token_t` with TTL and PQC signature.
- `cap_verify()` validates token signature, expiry, and required type.
- `cap_revoke()` marks a token as invalid.

## PQC

- `pqc_sign()` uses HMAC-SHA3-256 to sign messages (stub).
- `pqc_verify()` verifies HMAC signatures (stub).
- `quantum_sentinel_merkle()` computes Merkle root of PQC keys.

## Quantum

- `quantum_state_init()` creates a 2–8 qubit statevector.
- `quantum_apply_gate()` applies one gate operation.
- `quantum_measure_all()` produces a histogram of outcomes.

## Migration

- `migrate_backup()` writes a migration manifest to disk.
- `migrate_restore()` restores a manifest and reissues capability tokens.
- `migrate_validate()` enforces policy checks on manifests.

## Manifests & Policy

- `manifest_parse()` parses an app capability manifest.
- `manifest_validate()` ensures version compatibility.
- `manifest_to_caps()` converts manifest grants into tokens.
- `policy_validate_app()` rejects forbidden app categories.
