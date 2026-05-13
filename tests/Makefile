CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../include
LDFLAGS = -L../lib -lintentkernel -lm
TARGET = test_harness

all: $(TARGET)

$(TARGET): test_harness.c adversarial.c metrics.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) metrics.csv
