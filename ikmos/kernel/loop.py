from ikmos.core.security import classify_intent
from ikmos.kernel.context import KernelContext
from ikmos.kernel.parser import parse_command
from ikmos.kernel.router import route


class KernelLoop:
    def __init__(self) -> None:
        self.context = KernelContext()

    def handle(self, text: str) -> str:
        intent_cmd = parse_command(text)
        classify_intent(intent_cmd.raw)
        return route(intent_cmd.intent, intent_cmd.args)
