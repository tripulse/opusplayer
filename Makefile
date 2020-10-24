CFLAGS = -Ofast
LDLIBS+= $(shell pkg-config --cflags --libs opus opusfile portaudio-2.0)

BIN = opusplayer

build: player.c
	cpp -g -o "$(BIN).out" $(CFLAGS) $^ $(LDLIBS)

.PHONY: install clean
install:
	cp "$(BIN).out" /usr/bin/"$(BIN)"
	whereis $(BIN)
