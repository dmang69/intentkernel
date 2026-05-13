CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../../include
LDFLAGS = -L../../lib -lintentkernel
DAEMON = qproofd

all: $(DAEMON)

$(DAEMON): main.c proof.c
$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
rm -f $(DAEMON)
