<h1 align=center> opusplayer 🎵</h1>

<p align=center><i>Simple and lightweight application written in C++ for playing <a href="https://en.wikipedia.org/wiki/Opus_(audio_format)">Ogg OPUS</a> files. This is only can parse OGG Opus files, in future it may read OPUS codec data from other formats too (e.g mka).</i></p>

---

![C/C++ CI](https://github.com/tryamid/opusplayer/workflows/C/C++%20CI/badge.svg)
[![Build Status](https://travis-ci.org/tryamid/opusplayer.svg?branch=master)](https://travis-ci.org/tryamid/opusplayer)
![License](https://img.shields.io/badge/License-GPLv2-orange)
![Madewith](https://img.shields.io/badge/Made%20with-C%2B%2B-orange)

#### Dependency Tree 📜
Make sure that these dependencies are avialable in the environment you’re going to compile it,
on GNU/Linux it’s quite easy to install using a package manager. [`win32`](../../tree/win32) branch already includes
a complete setup. On, MinGW/Cygwin you need to do this manually.

- [![](https://img.shields.io/badge/opusfile-0.11-blue)](https://opus-codec.org/release/dev/2018/09/18/opusfile-0_11.html)
  - [![](https://img.shields.io/badge/libopus-1.3.1-blue)](https://opus-codec.org/release/stable/2019/04/12/libopus-1_3_1.html)
  - [![](https://img.shields.io/badge/libogg-1.3.4-blue)](https://www.xiph.org/downloads/)
- [![](https://img.shields.io/badge/libportaudio-19.06-blue)](http://portaudio.com/download.html)

#### Key features 🌟
- Cross platform (with PortAudio).
- File globbing (only on UNIX).<br/>
  <sup>queue multiple files at once for playback, like a playlist (e.g `*.opus` selects all of the OPUS files in the current directory).</sup>
- Very very lightweight.

### Installation

> This is the \*nix style of building the application from the source. If you're looking for the Windows way to build this, head to the
[`win32`](../../tree/win32) branch for that (MSVC toolchain and Visual Studio are required).
>
> All the libraries and DLLs are prebuilt for windows for the Win32 target (works for most processors supported by Windows), this reduces
> the fuss of linking libraries and DLLs.
> All the binaries and DLLs go to the `bin` directory.

This is the most traditional way to build up from source in \*NIX based systems:
```sh
make build
sudo make install
make clean # optional: cleanup isn't required unless cleaning mess
```
