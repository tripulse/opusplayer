# opusplayer

[![Build Status](https://travis-ci.org/nullvideo/opusplayer.svg?branch=master)](https://travis-ci.org/nullvideo/opusplayer)

A simple and lightweight application written in C for playing OPUS files (files are parsed as glob expressions). With the mixture of
**`opusfile`** and **PulseAudio** this application is made.

### Lacking features
- Play/Pause
- ncurses GUI

## Installation

Dependencies (`libopusfile`, `libopus`, `libpulse`) must be present in the system (either building from source or installing from the OS' software repository).

```sh
make build
sudo make install
make clean
```
