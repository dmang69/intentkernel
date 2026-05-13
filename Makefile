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
	user/qproofd \
	user/migrated

TOOLS = tools/intentkernel-migrate

.PHONY: all lib daemons tests test ikmos run tools clean

all: lib daemons tests ikmos tools

lib: $(LIB)

$(LIB): $(LIB_OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

daemons:
	@set -e; for dir in $(DAEMON_DIRS); do \
		$(MAKE) -C $$dir; \
	done

tests:
	$(MAKE) -C tests

test: tests
	./tests/test_harness
	./tests/test_migrate

ikmos: ikmos/requirements.txt
	python3 -m venv .venv
	./.venv/bin/pip install -r ikmos/requirements.txt

run:
	./scripts/run.sh

tools: $(TOOLS)

tools/intentkernel-migrate: tools/intentkernel-migrate.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $< -Llib -lintentkernel -lm

clean:
	rm -f lib/*.o $(LIB)
	@set -e; for dir in $(DAEMON_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	$(MAKE) -C tests clean
	rm -f $(TOOLS)
	rm -rf .venv
