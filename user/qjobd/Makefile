CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../../include
LDFLAGS = -L../../lib -lintentkernel -lm
DAEMON = qjobd

all: $(DAEMON)

$(DAEMON): main.c scheduler.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(DAEMON)
