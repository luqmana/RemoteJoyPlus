OUTPUT=remotejoy-plus
OBJS=main.o output_exts.o fast_events.o sdl_output.o ws_output.o rj_interface.o utils.o

CFLAGS=-std=c99 -O0 -Wall -g $(shell /usr/bin/sdl-config --cflags)

PREFIX=$(shell psp-config --pspdev-path 2> /dev/null)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^ $(shell /usr/bin/sdl-config --libs)

install: $(OUTPUT)
	@echo "Installing $(OUTPUT)..."
	@if ( test $(PREFIX) ); then { mkdir -p $(PREFIX)/bin && cp $(OUTPUT) $(PREFIX)/bin; } else { echo "Error: psp-config not found!"; exit 1; } fi
	@echo "Done!"

clean:
	rm -f $(OUTPUT) *.o