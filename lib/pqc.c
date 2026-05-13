#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "pqc.h"

void merkle_root(const u8 *leaves, size_t leaf_len, size_t leaf_count, u8 *out);

static void fill_random(u8 *buf, size_t len) {
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd >= 0) {
        ssize_t read_len = read(fd, buf, len);
        close(fd);
        if (read_len == (ssize_t)len) {
            return;
        }
    }
    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < len; i++) {
        buf[i] = (u8)(rand() & 0xFF);
    }
}

void pqc_init(pqc_ctx_t *ctx) {
    if (!ctx) {
        return;
    }
    fill_random(ctx->sk, sizeof(ctx->sk));
    memcpy(ctx->pk, ctx->sk, sizeof(ctx->pk));
    ctx->merkle_root = 0;
}

int pqc_sign(const u8 *msg, size_t msg_len, const pqc_ctx_t *sk, u8 *sig, size_t *sig_len) {
    if (!msg || !sk || !sig || !sig_len) {
        return -1;
    }
    size_t key_len = sizeof(sk->sk);
    size_t total_len = key_len + msg_len;
    u8 *buffer = (u8 *)malloc(total_len);
    if (!buffer) {
        return -1;
    }
    memcpy(buffer, sk->sk, key_len);
    memcpy(buffer + key_len, msg, msg_len);

    u8 hash[32];
    sha3_256(buffer, total_len, hash);
    free(buffer);

    for (size_t i = 0; i < CAP_SIG_LEN; i++) {
        sig[i] = hash[i % sizeof(hash)];
    }
    *sig_len = CAP_SIG_LEN;
    return 0;
}

int pqc_verify(const u8 *msg, size_t msg_len, const pqc_ctx_t *pk, const u8 *sig, size_t sig_len) {
    if (!msg || !pk || !sig) {
        return -1;
    }
    if (sig_len < CAP_SIG_LEN) {
        return -1;
    }
    size_t key_len = sizeof(pk->pk);
    size_t total_len = key_len + msg_len;
    u8 *buffer = (u8 *)malloc(total_len);
    if (!buffer) {
        return -1;
    }
    memcpy(buffer, pk->pk, key_len);
    memcpy(buffer + key_len, msg, msg_len);

    u8 hash[32];
    sha3_256(buffer, total_len, hash);
    free(buffer);

    for (size_t i = 0; i < CAP_SIG_LEN; i++) {
        if (sig[i] != hash[i % sizeof(hash)]) {
            return -1;
        }
    }
    return 0;
}

u64 quantum_sentinel_merkle(pqc_ctx_t *keys, int n) {
    if (!keys || n <= 0) {
        return 0;
    }
    u8 root[32];
    merkle_root(keys[0].pk, sizeof(keys[0].pk), (size_t)n, root);
    u64 value = 0;
    memcpy(&value, root, sizeof(value));
    for (int i = 0; i < n; i++) {
        keys[i].merkle_root = value;
    }
    return value;
}
