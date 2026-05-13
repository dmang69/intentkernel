CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../../include
LDFLAGS = -L../../lib -lintentkernel
DAEMON = intentd

all: $(DAEMON)

$(DAEMON): main.c classifier.c
$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
rm -f $(DAEMON)
