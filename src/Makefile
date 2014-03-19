all: fig tests

MV := mv
PKGS = gio-unix-2.0 gmodule-2.0

TMPL_DIR := $(shell pwd)/templates
PLGN_DIR := $(shell pwd)/plugins

OPTS = -O0
DEBUG = -ggdb -Wall -Werror
CFLAGS = \
	-fPIC \
	$(shell pkg-config --cflags $(PKGS)) \
	-DFIG_PLUGINS_DIR="\"$(PLGN_DIR)\"" \
	-DFIG_TEMPLATE_DIR="\"$(TMPL_DIR)\"" \
	-I. \
	-Iplugins
LIBS = $(shell pkg-config --libs $(PKGS)) \

OBJS = \
	fig-cli.o \
	fig-command.o \
	fig-command-info.o \
	fig-command-manager.o \
	fig-init-command.o \
	fig-template.o \
	fig-util.o

%.o: %.c %.h fig.h
	$(CC) $(DEBUG) $(OPTS) $(CFLAGS) -o $@.tmp -c $*.c
	$(MV) $@.tmp $@

fig: fig.c $(OBJS)
	$(CC) $(DEBUG) $(OPTS) $(CFLAGS) $(LIBS) -o $@.tmp fig.c $(OBJS)
	$(MV) $@.tmp $@

tests: $(OBJS) tests.c
	$(CC) -o $@.tmp $(DEBUG) $(OPTS) $(CFLAGS) $(LIBS) $(OBJS) tests.c
	$(MV) $@.tmp $@

clean:
	rm -f fig $(OBJS) tests *.tmp
