CXX = g++
CC = gcc
CFLAGS = -std=c11 -Ofast
CXXFLAGS = -std=c++17 -Ofast
LDLIBS+= $(shell pkg-config --cflags --libs opus opusfile libpulse libpulse-simple)


BINDIR = $(bin)
player: src/player.cc src/memory.h
	# '-g' flag enables debugging symbols to debug the code in GDB.
	$(CXX) -g -o $@.out $(CXXFLAGS) $^ $(LDLIBS)
