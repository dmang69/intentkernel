#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

make daemons ikmos

pids=()
start_daemon() {
    "$1" &
    pids+=("$!")
}

cleanup() {
    for pid in "${pids[@]}"; do
        if kill -0 "$pid" 2>/dev/null; then
            kill "$pid"
        fi
    done
}
trap cleanup EXIT

start_daemon "$ROOT/user/capd/capd"
start_daemon "$ROOT/user/intentd/intentd"
start_daemon "$ROOT/user/leasebroker/leasebroker"
start_daemon "$ROOT/user/eventscope/eventscope"
start_daemon "$ROOT/user/wasm_runtime/wasm_runtime"
start_daemon "$ROOT/user/ai_core/ai_core"
start_daemon "$ROOT/user/quantumd/quantumd"
start_daemon "$ROOT/user/qjobd/qjobd"
start_daemon "$ROOT/user/qsimd/qsimd"
start_daemon "$ROOT/user/qproofd/qproofd"

python3 -m ikmos.main
