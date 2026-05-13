from ikmos.kernel.registry import REGISTRY


def route(intent: str, args: str) -> str:
    handler = REGISTRY.get(intent, REGISTRY["unknown"])
    return handler(args)
