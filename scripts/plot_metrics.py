import csv
import sys
from pathlib import Path

try:
    from ikmos import __version__ as IK_VERSION
except Exception:
    IK_VERSION = "0.4.0"


def main() -> int:
    if len(sys.argv) < 2:
        print("Usage: plot_metrics.py <metrics.csv>")
        return 1

    csv_path = Path(sys.argv[1])
    if not csv_path.exists():
        print(f"Missing metrics file: {csv_path}")
        return 1

    rows = list(csv.DictReader(csv_path.open()))
    if not rows:
        print("No metrics to plot.")
        return 1

    try:
        import matplotlib.pyplot as plt
    except ImportError:
        print("matplotlib not available; skipping plot generation.")
        return 0

    metrics = rows[0]
    labels = [
        "denials",
        "escapes",
        "token_forgery",
        "syscall_abuse",
        "wasm_escape",
        "ipc_attack",
        "quantum_tamper",
    ]
    values = [float(metrics[label]) for label in labels]

    fig, ax = plt.subplots()
    ax.bar(labels, values)
    ax.set_title(f"IntentKernel v{IK_VERSION} Metrics")
    ax.set_ylabel("count")
    fig.autofmt_xdate(rotation=45)

    output = csv_path.with_suffix(".png")
    fig.tight_layout()
    fig.savefig(output)
    print(f"Wrote {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
