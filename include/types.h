#ifndef INTENTKERNEL_TYPES_H
#define INTENTKERNEL_TYPES_H

#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#define CAP_SIG_LEN 64

typedef struct {
    u64 slot;
    u8 sig[CAP_SIG_LEN];
    u64 expiry;
    u32 type;
    u32 flags;
} cap_token_t;

typedef u32 intent_id_t;

#define INTENT_PRINT            0x0001
#define INTENT_FILE_READ        0x0002
#define INTENT_FILE_WRITE       0x0003
#define INTENT_NETWORK_CONNECT  0x0004
#define INTENT_PROCESS_SPAWN    0x0005
#define INTENT_WASM_EXEC        0x0006
#define INTENT_QUANTUM_SUBMIT   0x5101
#define INTENT_QUANTUM_SIM      0x5102
#define INTENT_ROOT             0xFFFF

#define CAP_PRINT          0x0001
#define CAP_FILE_READ      0x0002
#define CAP_FILE_WRITE     0x0004
#define CAP_NETWORK        0x0008
#define CAP_PROCESS        0x0010
#define CAP_WASM_EXEC      0x0020
#define CAP_TRANSFERABLE   0x0040
#define CAP_REVOKABLE      0x0080
#define CAP_QUANTUM_SUBMIT 0x0100
#define CAP_ROOT           0xFFFF
#define CAP_FLAG_MIGRATION_PERSIST 0x0001

typedef struct {
    cap_token_t parent;
    u64 expiry;
    u8 sig[CAP_SIG_LEN];
} lease_t;

typedef struct {
    u32 type;
    u64 sender;
    u64 timestamp;
    u8 data[256];
    u32 len;
} ipc_msg_t;

#define MSG_TOKEN_ISSUE      1
#define MSG_TOKEN_REPLY      2
#define MSG_TOKEN_REVOKE     3
#define MSG_INTENT_CLASSIFY  4
#define MSG_INTENT_REPLY     5
#define MSG_LEASE_REQUEST    6
#define MSG_LEASE_REPLY      7
#define MSG_EVENT_AUDIT      8
#define MSG_WASM_EXEC        9
#define MSG_AI_CLASSIFY      10
#define MSG_QUANTUM_SUBMIT   11
#define MSG_QUANTUM_RESULT   12
#define MSG_JOB_ENQUEUE      13
#define MSG_PROOF_SIGN       14
#define MSG_MIGRATE_REQUEST  15
#define MSG_MIGRATE_REPLY    16

#endif
