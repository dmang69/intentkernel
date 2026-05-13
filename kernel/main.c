#include <stdio.h>
#include <unistd.h>
#include "intentkernel.h"

int dispatch_intent(intent_id_t intent, cap_token_t *cap);

int main(void) {
    printf("IntentKernel simulated kernel starting...\n");
    for (;;) {
        sleep(1);
    }
    return 0;
}
