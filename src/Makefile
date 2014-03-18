all: fig plugins/fig-init-command.so

PKGS = gio-2.0 gmodule-2.0

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
	fig-command.o \
	fig-command-info.o \
	fig-plugin.o \
	fig-template.o

%.o: %.c %.h fig.h
	$(CC) $(DEBUG) $(OPTS) $(CFLAGS) -o $@ -c $*.c

fig: fig.c libfig.so
	$(CC) $(DEBUG) $(OPTS) $(CFLAGS) $(LIBS) -o $@ fig.c $(OBJS)

libfig.so: $(OBJS)
	$(CC) -shared $(DEBUG) $(OPTS) $(LIBS) $(OBJS) -o $@

plugins/fig-init-command.so: libfig.so plugins/fig-init-command.c plugins/fig-init-command.h
	$(CC) $(DEBUG) $(CFLAGS) $(LIBS) -shared -o $@ plugins/fig-init-command.c

clean:
	rm -f fig plugins/fig-init-command.so *.o libfig.so
