# IntentKernel Thesis Appendix Template

## Abstract

_Summarize the intent-driven capability model, evaluation goals, and results._

## Implementation Summary

- User-space kernel simulation with IPC-enforced intents.
- PQC stub signatures with SHA3-256 hashing.
- Quantum workflow pipeline (submit → simulate → sign).
- IK-MOS Python shell for interaction.

## Evaluation Metrics

- Denial rate (capability rejection percentage)
- IPC latency
- Capability issuance latency
- Intent classification accuracy

## Reproducibility

```
make all
make run
make test
python3 scripts/plot_metrics.py metrics.csv
```
