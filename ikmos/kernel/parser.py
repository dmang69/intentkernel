from dataclasses import dataclass


@dataclass
class IntentCommand:
    raw: str
    intent: str
    args: str


def parse_command(text: str) -> IntentCommand:
    lowered = text.lower()
    if lowered.startswith("read"):
        return IntentCommand(raw=text, intent="file_read", args=text[4:].strip())
    if lowered.startswith("write"):
        return IntentCommand(raw=text, intent="file_write", args=text[5:].strip())
    if lowered.startswith("quantum"):
        return IntentCommand(raw=text, intent="quantum", args=text[len("quantum"):].strip())
    if lowered.startswith("network"):
        return IntentCommand(raw=text, intent="network", args=text[len("network"):].strip())
    if lowered.startswith("help"):
        return IntentCommand(raw=text, intent="help", args="")
    return IntentCommand(raw=text, intent="unknown", args=text)
