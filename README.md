# opusplayer 🎵

[![Build Status](https://travis-ci.org/tryamid/opusplayer.svg?branch=master)](https://travis-ci.org/tryamid/opusplayer)
![License](https://img.shields.io/badge/License-GPLv2-orange)
![Madewith](https://img.shields.io/badge/Made%20with-C%2B%2B-orange)

#### Dependency Tree 📜
- ![](https://img.shields.io/badge/opusfile-0.11-blue)
  - ![](https://img.shields.io/badge/libopus-1.3.1-blue)
  - ![](https://img.shields.io/badge/libogg-1.3.4-blue)
- ![](https://img.shields.io/badge/libportaudio-19.06-blue)

Simple and lightweight application written in C++ for playing *OggOPUS* files. This is only can parse OGGOpus files, in future it may read OPUS data from other formats too.

#### Key features 🌟
- Cross platform (with PortAudio).
- File globbing (only on UNIX).
- Very very lightweight.

## Installation

> This is the \*nix style of building the application from the source. If you're looking for the Windows way to build this, head to the
[`win32`](../../tree/win32) branch for that (MSVC toolchain and Visual Studio are required).
>
> All the libraries and DLLs are prebuilt for windows for the Win32 target (works for most processors supported by Windows), this reduces
> the fuss of linking libraries and DLLs.
> All the binaries and DLLs go to the `bin` directory.

In order to have a sucessful build you *must* satisfy these depedencies:
- [`opusfile`](https://opus-codec.org/release/dev/2018/09/18/opusfile-0_11.html)
  - [`libopus`](https://opus-codec.org/release/stable/2019/04/12/libopus-1_3_1.html)
  - [`libogg`](https://www.xiph.org/downloads/)
- [`portaudio`](http://portaudio.com/download.html)


```sh
make build
sudo make install # Optional: user may not install binaries
make clean # Optional: cleanup isn't required unless cleaning mess
```
