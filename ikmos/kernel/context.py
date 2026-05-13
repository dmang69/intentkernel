from dataclasses import dataclass, field
from typing import Dict, Any


@dataclass
class KernelContext:
    user: str = "user"
    working_dir: str = "."
    vars: Dict[str, Any] = field(default_factory=dict)
