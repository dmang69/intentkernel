# IntentKernel v0.4 — Unified Prototype

IntentKernel is a research prototype demonstrating **intent-driven, capability-based secure operating system architecture** with post-quantum cryptography, quantum computing integration, WASM sandboxing, and ML-based intent classification.

**Target**: User-space prototype runnable on Linux, suitable for thesis demonstration, security evaluation, and metrics collection.

**Thesis claim**: _Event-scoped capability enforcement can be demonstrated through a user-space compatibility layer, producing measurable security improvement with quantum-safe attestation._

## Highlights

- Event-scoped capabilities issued by `capd`, validated by a simulated kernel filter.
- Post-quantum cryptography stubs (ML-DSA-87 + SHA3-256) and Merkle-based sentinel.
- Quantum workflow pipeline (`quantumd` → `qjobd` → `qsimd` → `qproofd`).
- IK-MOS Python shell for intent-driven interaction.
- Adversarial test harness with CSV metrics output.
- Migration architecture with manifest-based backup/restore tooling.

## Build

```bash
make all
```

## Run

```bash
make run
```

## Test

```bash
make test
./tests/test_harness
python3 scripts/plot_metrics.py metrics.csv
python3 scripts/security_report.py metrics.csv
```

## Repository Layout

```
include/     Shared C headers
lib/         IPC, capability, PQC, CBOR, quantum, ML libraries
kernel/      Kernel simulation layer
user/        User-space daemons
ikmos/       Python meta-OS shell
tests/       Adversarial test harness
scripts/     Build/run/test helpers
docs/        Architecture + thesis templates
```

## Thesis References

- `docs/ARCHITECTURE.md` — Unified architecture overview
- `docs/API_SPEC.md` — Core API specification
- `docs/MIGRATION.md` — Migration engine and manifest format
- `docs/THESIS_TEMPLATE.md` — Thesis appendix template

## License

Apache 2.0. See `LICENSE`.
