
def help_text(_: str = "") -> str:
    return "Available commands: read, write, network, quantum, help"


def unknown(cmd: str) -> str:
    return f"Unknown command: {cmd.strip()}"
