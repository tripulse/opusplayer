CXX = g++
CXXFLAGS = -std=c++17 -Ofast
LDLIBS+= $(shell pkg-config --cflags --libs opus opusfile portaudio-2.0)

BIN = opusplayer

build: player.cc
	$(CXX) -g -o "$(BIN).out" $(CXXFLAGS) $^ $(LDLIBS)

.PHONY: install clean
install:
	cp "$(BIN).out" /usr/bin/"$(BIN)"
	whereis $(BIN)

clean:
	rm -r $(BINDIR)
