all: fig plugins/fig-init.so

FILES = fig*.c fig*.h

PKGS = gio-2.0 gmodule-2.0

fig: $(FILES)
	$(CC) -fPIC -g -o $@ -Wall $(FILES) $(shell pkg-config --cflags --libs $(PKGS)) -DFIG_PLUGINS_DIR="\"plugins\""

plugins/fig-init.so: plugins/fig-init.c
	$(CC) -fPIC -g -shared -o $@ -Wall plugins/fig-init.c $(shell pkg-config --cflags --libs $(PKGS)) -DFIG_PLUGINS_DIR="\"plugins\"" -I.

clean:
	rm -f fig plugins/fig-init.so
