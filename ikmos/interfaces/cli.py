from ikmos.kernel.loop import KernelLoop


def run_cli() -> int:
    kernel = KernelLoop()
    print("IK-MOS shell ready. Type 'help' for commands, 'exit' to quit.")
    while True:
        try:
            line = input("ikmos> ")
        except EOFError:
            break
        if line.strip() in {"exit", "quit"}:
            break
        response = kernel.handle(line)
        print(response)
    return 0
