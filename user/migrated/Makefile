CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../../include
LDFLAGS = -L../../lib -lintentkernel -lm
DAEMON = migrated

all: $(DAEMON)

$(DAEMON): main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(DAEMON)
