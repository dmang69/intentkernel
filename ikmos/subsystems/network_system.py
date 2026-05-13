from ikmos.core.security import request_capability


def fetch(target: str) -> str:
    request_capability("CAP_NETWORK", target)
    return f"[ikmos] would perform network request: {target.strip() or '<empty>'}"
