# opusplayer

[![Build Status](https://travis-ci.org/nullvideo/opusplayer.svg?branch=master)](https://travis-ci.org/nullvideo/opusplayer)

A simple and lightweight application written in C for playing OPUS files (files are parsed as glob expressions). With the mixture of **`opusfile`** and **PortAudio** this application is made (this is somewhat inferior to SOX's `play`).

### Key features
- OPUS decoding via `libopusfile`.
- Cross-platform audio API.
- Glob expressions to select files.

## Installation

> This is the \*nix style of building the application from the source. If you're looking for the Windows way to build this, head to the
[`win32`](../../tree/win32) branch for that (MSVC toolchain and Visual Studio are required).
>
> All the libraries and DLLs are prebuilt for windows for the Win32 target (works for most processors supported by Windows), this reduces
> the fuss of linking libraries and DLLs.
> All the binaries and DLLs go to the `bin` directory.

In order to have a sucessful build you *must* satisfy these depedencies:
- [opusfile](https://opus-codec.org/release/dev/2018/09/18/opusfile-0_11.html)
  - [libopus](https://opus-codec.org/release/stable/2019/04/12/libopus-1_3_1.html)
  - [libogg](https://www.xiph.org/downloads/)
- [libportaudio](http://portaudio.com/download.html)


```sh
make build
sudo make install # Optional: user may not install binaries
make clean # Optional: cleanup isn't required unless cleaning mess
```
