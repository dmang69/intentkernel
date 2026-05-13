CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../include
LDFLAGS = -L../lib -lintentkernel -lm
TARGETS = test_harness test_migrate

all: $(TARGETS)

test_harness: test_harness.c adversarial.c metrics.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_migrate: test_migrate.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGETS) metrics.csv
