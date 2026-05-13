#ifndef INTENTKERNEL_IPC_H
#define INTENTKERNEL_IPC_H

#include <stddef.h>
#include "types.h"

#define CH_CAPD        1
#define CH_INTENTD     2
#define CH_LEASEBROKER 3
#define CH_EVENTSCOPE  4
#define CH_WASM        5
#define CH_AI_CORE     6
#define CH_QUANTUMD    7
#define CH_QSIMD       8
#define CH_QJOBD       9
#define CH_QPROOFD     10
#define CH_MIGRATED    11

int ipc_init(void);
int ipc_send(int channel, const ipc_msg_t *msg);
int ipc_recv(int channel, ipc_msg_t *msg, int timeout_ms);
int ipc_channel_create(int channel_id, const char *name);

#endif
