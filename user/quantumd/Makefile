CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../../include
LDFLAGS = -L../../lib -lintentkernel
DAEMON = quantumd

all: $(DAEMON)

$(DAEMON): main.c backend.c
$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
rm -f $(DAEMON)
