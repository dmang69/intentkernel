import os
import socket
import struct
import time
from ikmos.core.config import CONFIG

IPC_STRUCT = struct.Struct("<IQQ256sI")


def _socket_path(channel: int) -> str:
    return os.path.join(CONFIG["ipc_dir"], f"ch_{channel}")


def _send_ipc(channel: int, msg_type: int, payload: bytes) -> None:
    data = payload[:256].ljust(256, b"\x00")
    packet = IPC_STRUCT.pack(msg_type, 0, int(time.time()), data, len(payload))
    path = _socket_path(channel)
    if not os.path.exists(path):
        return
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
    try:
        sock.sendto(packet, path)
    finally:
        sock.close()


def request_capability(cap_type: str, context: str) -> None:
    payload = cap_type.encode()[:32].ljust(32, b"\x00") + context.encode()[:224]
    _send_ipc(1, 1, payload)


def classify_intent(text: str) -> None:
    _send_ipc(2, 4, text.encode())
