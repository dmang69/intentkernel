_state = {}


def set_value(data: str) -> str:
    parts = data.split("=", 1)
    if len(parts) == 2:
        _state[parts[0].strip()] = parts[1].strip()
    return "[ikmos] state updated"


def get_value(key: str) -> str:
    return _state.get(key.strip(), "<unset>")
