from ikmos.core.security import request_migration


def handle(args: str) -> str:
    parts = args.split()
    if not parts:
        return "Usage: migrate <backup|restore|validate|status|list-apps|list-caps> [arg]"
    action = parts[0]
    if action == "list":
        action = "list-apps"
    payload = " ".join(parts[1:]) if len(parts) > 1 else ""
    request_migration(action, payload)
    return f"[ikmos] migrate {action} requested"
