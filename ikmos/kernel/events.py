from typing import Callable, Dict, List, Any


class EventBus:
    def __init__(self) -> None:
        self._subscribers: Dict[str, List[Callable[[Any], None]]] = {}

    def subscribe(self, event: str, handler: Callable[[Any], None]) -> None:
        self._subscribers.setdefault(event, []).append(handler)

    def emit(self, event: str, payload: Any) -> None:
        for handler in self._subscribers.get(event, []):
            handler(payload)
