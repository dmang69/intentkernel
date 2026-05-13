#include <stdlib.h>
#include <string.h>
#include "pqc.h"

void merkle_root(const u8 *leaves, size_t leaf_len, size_t leaf_count, u8 *out) {
    if (leaf_count == 0) {
        memset(out, 0, 32);
        return;
    }

    u8 *level = (u8 *)malloc(leaf_count * 32);
    if (!level) {
        memset(out, 0, 32);
        return;
    }

    for (size_t i = 0; i < leaf_count; i++) {
        sha3_256(leaves + (i * leaf_len), leaf_len, level + (i * 32));
    }

    size_t count = leaf_count;
    u8 buffer[64];
    while (count > 1) {
        size_t next = (count + 1) / 2;
        for (size_t i = 0; i < next; i++) {
            size_t left = i * 2;
            size_t right = left + 1;
            memcpy(buffer, level + (left * 32), 32);
            if (right < count) {
                memcpy(buffer + 32, level + (right * 32), 32);
            } else {
                memcpy(buffer + 32, level + (left * 32), 32);
            }
            sha3_256(buffer, sizeof(buffer), level + (i * 32));
        }
        count = next;
    }

    memcpy(out, level, 32);
    free(level);
}
