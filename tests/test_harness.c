#include <stdio.h>
#include "adversarial.h"

int main(void) {
    metrics_t metrics;
    metrics_init(&metrics);

    run_adversarial_tests(&metrics);

    metrics_write_csv(&metrics, "metrics.csv");
    printf("Metrics written to metrics.csv\n");
    return 0;
}
