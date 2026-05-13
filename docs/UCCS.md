# UCCS Overview

The Universal Capability Computing Substrate (UCCS) defines the hardware-agnostic execution contract for IntentKernel. This prototype focuses on user-space enforcement while maintaining the same capability semantics.

## Prototype Scope

- Capability verification is enforced in the kernel simulation filter.
- Syscall intent mapping is modeled via daemon IPC.
- Capability tokens follow an event-scoped lifecycle with short TTLs.
