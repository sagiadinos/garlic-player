## MDK: Multimedia Development Kit

### Features
- Simple and powerful API set
- Cross platform: Windows, UWP, Linux, macOS, Android, iOS, Raspberry Pi
- Hardware accelerated decoding and 0-copy GPU rendering for all supported platforms
- OpenGL rendering w/ or w/o user provided context
- Ingegrated with any gui toolkits via OpenGL (Qt, SDL, glfw, SFML etc.) easily
- Seamless/Gapless media and bitrate switch for any media
- Configurable FFmpeg runtime libraries via environment var AVUTIL_LIB, AVCODEC_LIB, AVFORMAT_LIB, AVFILTER_LIB, SWRESAMPLE_LIB, or SetGlobalOption() with key avutil_lib, avcodec_lib, avformat_lib, swresample_lib, avfilter_lib 


## About SDK for Linux
SDK is built with
- ffmpeg: https://sourceforge.net/projects/avbuild/files/linux/ffmpeg-git-linux-clang-lite.tar.xz/download

SDK can be used by any C or C++11 compiler, e.g. g++, clang

### Runtime Requirements
libva2, libva-x11-2, libva-drm2, libvdpau1, libasound0, libpulse0, libwayland-client0, libgbm1, libegl1-mesa, libgl1-mesa-glx

Optional: libopenal1, libsdl2(sdlplay)

### Environment Vars:
- GL_EGL: 0 = use glx context, 1 = use egl context (if created by mdk)
- GL_ES: 0 = use opengl, 1 = use opengl es (if created by mdk)
- GL_VDPAU: video = interop with video surface, output = interop with output surface, pixmap = interop with x11 pixmap
- GL_VAAPI: x11 = interop with glx, drm = interop with drm prime, drm2 = interop with drm prime2
- CUDA_STREAM: 0/1
- CUDA_PBO: 0/1
- CUDA_HOST: 0/1
- CUDA_DEVICE: N

### Decoders:
- FFmpeg, VDPAU, VAAPI, CUDA, QSV(not tested), NVDEC
- command line: -c:v decodername

### Examples
GL Context
- Created by MDK: glfwplay -gfxthread, glfwplay -es -gfxthread, mdkplay, x11win
- Foreign Context: glfwplay, multiplayers, multiwindows (via glfw), sdlplay(via sdl)

Gapless Playback for Any Media:
- mdkplay(or glfwplay/window/sdlplay) file file2 ...

N players for 1 video: multiplayers -share -win N url

N videos and N players: multiplayers -share url1 url2 ... urlN

N videos renderers for 1 player: multiwidnows url


## Source code:
- some examples using mdk sdk: https://github.com/wang-bin/mdk-examples
- QtMultimedia plugin: https://github.com/wang-bin/qtmultimedia-plugins-mdk

Copyright (c) 2016-2019 WangBin(the author of QtAV) <wbsecg1 at gmail.com>
Free for non-commercial use.
