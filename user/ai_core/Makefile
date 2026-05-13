CC = gcc
CFLAGS = -Wall -Wextra -O2 -I../../include
LDFLAGS = -L../../lib -lintentkernel -lm
DAEMON = ai_core

all: $(DAEMON)

$(DAEMON): main.c ai_classifier.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(DAEMON)
