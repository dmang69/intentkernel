#include <stddef.h>
#include <strings.h>
#include "policy.h"

static const char *forbidden_categories[] = {
    "adware",
    "spyware",
    "bloatware",
    "cryptominer",
    "toolbar"
};

int policy_is_forbidden(const char *category) {
    if (!category || category[0] == '\0') {
        return 0;
    }
    for (size_t i = 0; i < sizeof(forbidden_categories) / sizeof(forbidden_categories[0]); i++) {
        if (strcasecmp(category, forbidden_categories[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int policy_validate_app(const char *category) {
    return policy_is_forbidden(category) ? -1 : 0;
}
