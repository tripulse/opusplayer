# opusplayer

[![Build Status](https://travis-ci.org/nullvideo/opusplayer.svg?branch=master)](https://travis-ci.org/nullvideo/opusplayer)

A simple and lightweight application written in C for playing OPUS files (files are parsed as glob expressions). With the mixture of **`opusfile`** and **PortAudio** this application is made (this is somewhat inferior to SOX's `play`).

### Key features
- OPUS decoding via `libopusfile`.
- Cross-platform audio API.
- Glob expressions to select files.

## Installation

Dependencies (`libopusfile`, `libopus`, `libportaudio`) must be present in the system (either building from source or installing from the OS' software repository).

```sh
make build
sudo make install
make clean
```
