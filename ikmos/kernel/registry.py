from typing import Callable, Dict
from ikmos.subsystems import file_system, network_system, quantum_system, system_ops


REGISTRY: Dict[str, Callable[[str], str]] = {
    "file_read": file_system.read,
    "file_write": file_system.write,
    "network": network_system.fetch,
    "quantum": quantum_system.submit,
    "help": system_ops.help_text,
    "unknown": system_ops.unknown,
}
