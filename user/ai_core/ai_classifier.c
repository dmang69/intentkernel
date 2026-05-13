#include <string.h>
#include "intentkernel.h"

int ai_classify_risk(const char *text) {
    if (!text) {
        return 2;
    }
    if (strstr(text, "gov") || strstr(text, "policy")) {
        return 1;
    }
    if (strstr(text, "measure") || strstr(text, "metrics")) {
        return 3;
    }
    if (strstr(text, "manage") || strstr(text, "risk")) {
        return 4;
    }
    return 2;
}
