CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../../include
LDFLAGS = -L../../lib -lintentkernel -lm
DAEMON = leasebroker

all: $(DAEMON)

$(DAEMON): main.c lease.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(DAEMON)
