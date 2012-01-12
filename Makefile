OUTPUT=remotejoy-plus
OBJS=main.o client_exts.o fast_events.o sdl_client.o rj_interface.o utils.o ws_client.o

CFLAGS=-std=c99 -O2 -Wall -g $(shell /usr/bin/sdl-config --cflags) -I../WebSockets

PREFIX=$(shell psp-config --pspdev-path 2> /dev/null)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^ $(shell /usr/bin/sdl-config --libs) -L../WebSockets -lwebsockets

install: $(OUTPUT)
	@echo "Installing $(OUTPUT)..."
	@if ( test $(PREFIX) ); then { mkdir -p $(PREFIX)/bin && cp $(OUTPUT) $(PREFIX)/bin; } else { echo "Error: psp-config not found!"; exit 1; } fi
	@echo "Done!"

clean:
	rm -f $(OUTPUT) *.o
