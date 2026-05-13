#include <string.h>
#include "pqc.h"

typedef struct {
    u64 st[25];
    unsigned int mdlen;
    unsigned int rsiz;
    unsigned int pt;
} sha3_ctx_t;

static const u64 keccakf_rndc[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL,
    0x800000000000808aULL, 0x8000000080008000ULL,
    0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008aULL, 0x0000000000000088ULL,
    0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL,
    0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL,
    0x8000000080008081ULL, 0x8000000000008080ULL,
    0x0000000080000001ULL, 0x8000000080008008ULL
};

static const unsigned int keccakf_rotc[24] = {
    1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 2, 14,
    27, 41, 56, 8, 25, 43, 62, 18, 39, 61, 20, 44
};

static const unsigned int keccakf_piln[24] = {
    10, 7, 11, 17, 18, 3, 5, 16, 8, 21, 24, 4,
    15, 23, 19, 13, 12, 2, 20, 14, 22, 9, 6, 1
};

static inline u64 rol64(u64 x, unsigned int y) {
    return (x << y) | (x >> (64 - y));
}

static void keccakf(u64 st[25]) {
    u64 bc[5];
    u64 t;

    for (int r = 0; r < 24; r++) {
        for (int i = 0; i < 5; i++) {
            bc[i] = st[i] ^ st[i + 5] ^ st[i + 10] ^ st[i + 15] ^ st[i + 20];
        }
        for (int i = 0; i < 5; i++) {
            t = bc[(i + 4) % 5] ^ rol64(bc[(i + 1) % 5], 1);
            for (int j = 0; j < 25; j += 5) {
                st[j + i] ^= t;
            }
        }
        t = st[1];
        for (int i = 0; i < 24; i++) {
            int j = keccakf_piln[i];
            bc[0] = st[j];
            st[j] = rol64(t, keccakf_rotc[i]);
            t = bc[0];
        }
        for (int j = 0; j < 25; j += 5) {
            for (int i = 0; i < 5; i++) {
                bc[i] = st[j + i];
            }
            for (int i = 0; i < 5; i++) {
                st[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
            }
        }
        st[0] ^= keccakf_rndc[r];
    }
}

static void sha3_init(sha3_ctx_t *c, int mdlen) {
    memset(c, 0, sizeof(*c));
    c->mdlen = mdlen;
    c->rsiz = 200 - 2 * mdlen;
    c->pt = 0;
}

static void sha3_update(sha3_ctx_t *c, const void *data, size_t len) {
    size_t i;
    size_t j = c->pt;

    for (i = 0; i < len; i++) {
        ((u8 *)c->st)[j++] ^= ((const u8 *)data)[i];
        if (j >= c->rsiz) {
            keccakf(c->st);
            j = 0;
        }
    }
    c->pt = (unsigned int)j;
}

static void sha3_final(void *md, sha3_ctx_t *c) {
    ((u8 *)c->st)[c->pt] ^= 0x06;
    ((u8 *)c->st)[c->rsiz - 1] ^= 0x80;
    keccakf(c->st);
    memcpy(md, c->st, c->mdlen);
}

void sha3_256(const u8 *in, size_t inlen, u8 *out) {
    sha3_ctx_t ctx;
    sha3_init(&ctx, 32);
    sha3_update(&ctx, in, inlen);
    sha3_final(out, &ctx);
}
