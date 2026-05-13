CC ?= gcc
AR ?= ar
CFLAGS ?= -Wall -Wextra -O2 -Iinclude
LIB = lib/libintentkernel.a
LIB_SRCS = $(wildcard lib/*.c)
LIB_OBJS = $(LIB_SRCS:.c=.o)

DAEMON_DIRS = \
	user/capd \
	user/intentd \
	user/leasebroker \
	user/eventscope \
	user/wasm_runtime \
	user/ai_core \
	user/quantumd \
	user/qsimd \
	user/qjobd \
	user/qproofd

.PHONY: all lib daemons tests ikmos run clean

all: lib daemons tests ikmos

lib: $(LIB)

$(LIB): $(LIB_OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

daemons:
	@for dir in $(DAEMON_DIRS); do \
		$(MAKE) -C $$dir; \
	done

tests:
	$(MAKE) -C tests

ikmos: ikmos/requirements.txt
	python3 -m venv .venv
	./.venv/bin/pip install -r ikmos/requirements.txt

run:
	./scripts/run.sh

clean:
	rm -f lib/*.o $(LIB)
	@for dir in $(DAEMON_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	$(MAKE) -C tests clean
	rm -rf .venv
