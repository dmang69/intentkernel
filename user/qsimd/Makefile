CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../../include
LDFLAGS = -L../../lib -lintentkernel
DAEMON = qsimd

all: $(DAEMON)

$(DAEMON): main.c sim.c
$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
rm -f $(DAEMON)
