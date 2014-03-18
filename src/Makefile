all: fig plugins/fig-init-command.so

FILES = fig*.c fig*.h

PKGS = gio-2.0 gmodule-2.0

TMPL_DIR := $(shell pwd)/templates
PLGN_DIR := $(shell pwd)/plugins

fig: $(FILES)
	$(CC) -fPIC -g -o $@ -Wall $(FILES) $(shell pkg-config --cflags --libs $(PKGS)) -DFIG_PLUGINS_DIR="\"$(PLGN_DIR)\"" -DFIG_TEMPLATE_DIR="\"$(TMPL_DIR)\""

plugins/fig-init-command.so: plugins/fig-init-command.c plugins/fig-init-command.h
	$(CC) -fPIC -g -shared -o $@ -Wall plugins/fig-init-command.c $(shell pkg-config --cflags --libs $(PKGS)) -DFIG_PLUGINS_DIR="\"plugins\"" -I.

clean:
	rm -f fig plugins/fig-init.so
