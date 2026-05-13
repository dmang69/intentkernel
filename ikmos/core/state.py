class State:
    def __init__(self) -> None:
        self.history = []

    def record(self, command: str) -> None:
        self.history.append(command)
