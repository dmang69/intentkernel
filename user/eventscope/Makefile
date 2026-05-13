CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../../include
LDFLAGS = -L../../lib -lintentkernel
DAEMON = eventscope

all: $(DAEMON)

$(DAEMON): main.c monitor.c
$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
rm -f $(DAEMON)
