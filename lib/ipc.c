#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>
#include "ipc.h"
#include "cbor.h"

#define IPC_DIR "/tmp/intentkernel"
#define IPC_MAX_CHANNELS 32

static int channel_fds[IPC_MAX_CHANNELS];
static int ipc_initialized = 0;

static void ipc_path(char *buf, size_t len, int channel) {
    snprintf(buf, len, "%s/ch_%d", IPC_DIR, channel);
}

int ipc_init(void) {
    if (ipc_initialized) {
        return 0;
    }
    mkdir(IPC_DIR, 0700);
    for (int i = 0; i < IPC_MAX_CHANNELS; i++) {
        channel_fds[i] = -1;
    }
    ipc_initialized = 1;
    return 0;
}

int ipc_channel_create(int channel_id, const char *name) {
    (void)name;
    if (channel_id <= 0 || channel_id >= IPC_MAX_CHANNELS) {
        return -1;
    }
    if (!ipc_initialized) {
        ipc_init();
    }

    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0) {
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    ipc_path(addr.sun_path, sizeof(addr.sun_path), channel_id);
    unlink(addr.sun_path);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        close(fd);
        return -1;
    }

    channel_fds[channel_id] = fd;
    return 0;
}

int ipc_send(int channel, const ipc_msg_t *msg) {
    if (!msg) {
        return -1;
    }
    if (!ipc_initialized) {
        ipc_init();
    }
    if (channel <= 0 || channel >= IPC_MAX_CHANNELS) {
        return -1;
    }

    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0) {
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    ipc_path(addr.sun_path, sizeof(addr.sun_path), channel);

    u8 buffer[sizeof(ipc_msg_t)];
    size_t len = sizeof(buffer);
    if (cbor_encode_ipc_msg(msg, buffer, &len) != 0) {
        close(fd);
        return -1;
    }

    ssize_t sent = sendto(fd, buffer, len, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(fd);
    return (sent == (ssize_t)len) ? 0 : -1;
}

int ipc_recv(int channel, ipc_msg_t *msg, int timeout_ms) {
    if (!msg) {
        return -1;
    }
    if (!ipc_initialized) {
        ipc_init();
    }
    if (channel <= 0 || channel >= IPC_MAX_CHANNELS) {
        return -1;
    }

    int fd = channel_fds[channel];
    if (fd < 0) {
        errno = ENOTCONN;
        return -1;
    }

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    int rc = poll(&pfd, 1, timeout_ms);
    if (rc <= 0) {
        return rc;
    }

    u8 buffer[sizeof(ipc_msg_t)];
    ssize_t read_len = recv(fd, buffer, sizeof(buffer), 0);
    if (read_len < 0) {
        return -1;
    }
    return cbor_decode_ipc_msg(buffer, (size_t)read_len, msg);
}
