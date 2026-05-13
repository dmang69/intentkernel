from ikmos.core.security import request_capability


def read(path: str) -> str:
    request_capability("CAP_FILE_READ", path)
    return f"[ikmos] would read file: {path.strip() or '<empty>'}"


def write(path: str) -> str:
    request_capability("CAP_FILE_WRITE", path)
    return f"[ikmos] would write file: {path.strip() or '<empty>'}"
