# IntentKernel v0.3 Architecture

IntentKernel is a user-space prototype demonstrating event-scoped capability enforcement with post-quantum cryptography and quantum workflow integration.

## Components

- **Kernel simulation** (`kernel/`): routes intents through a capability filter.
- **Shared libraries** (`lib/`): IPC, capability issuance, PQC stubs, CBOR serialization, quantum sim, ML classification.
- **Daemons** (`user/`): intent classification, capability issuance, lease broker, eventscope, WASM runtime, AI core, quantum pipeline.
- **IK-MOS shell** (`ikmos/`): interactive meta-OS shell that issues intents and requests capabilities.

## Flow

1. IK-MOS captures a user command and classifies intent.
2. `intentd` returns an intent ID and confidence score.
3. `capd` issues a scoped capability token.
4. The simulated kernel filter validates the intent + token pair.
5. Daemons execute the requested action with capability enforcement.

## Security Model

- Zero ambient authority: all actions require explicit capabilities.
- Event-scoped tokens: short TTL, single purpose.
- Post-quantum stubs: ML-DSA-87 signatures and SHA3-256 hashing.
