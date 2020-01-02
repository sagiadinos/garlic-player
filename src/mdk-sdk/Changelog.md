Change log:

0.2.3 - 2019-07-01

- muxer: skip unwanted stream packets
- GL:
    - support gbrp formats
    - improve yuv to rgb
    - xyz gamma correction
- generic color space, trc, primaries support. done in renderer, demuxers and decoders(FFmpeg, VT, AMediaCodec, MFT)
- android AudioTrack: do not use deprecated 7.1 channel value, which may not work in new OSes
- FFmpeg:
    - fix sw format after seek
    - check hw pixfmt of decoded frame, fix sw fallback
    - support avcodec options via property, e.g. FFmpeg:thread=2
- fix annexb check for CUDA, MFT, AMediaCodec decoder
- try next decoder set by user if failed to decode a frame
- AMediaCodec decoder:
    - profile, level and capabilities check before using a codec
    - fix nv12 plane line size in copy mode
    - async mode support
    - detect decode error
- VT decoder(VideoToolbox): add property "copy"
- MFT decoder:
    - apply property "copy", "pool" on the fly
    - fix annexb check
- raspberry pi:
    - enable builtin mmal decoder. decoder name is "mmal", while FFmpeg's is "MMAL"


0.2.2 - 2019-06-05

- API changes:
    - add Player.record()   to record while playing
    - add SetGlobalOption(), can be used to set ffmpeg runtime paths via key "avutil_lib", "avcodec_lib" etc.
- ABI break: no
- NEW:
    - add NV24/42
    - GL: support xyz12le/be
    - MFT: support D3D11 for win7
    - support color space, primaries, trc, range, hdr metadata for VideoFrame, MediaInfo, decoders(except android) etc.
    - support HDR10 for windows GLES2/3(ANGLE) via environment var "D3D11_GL=eglpbuf"
    - support muxing
- fix m3u check
- GL: scale high depth color depends on channel shift
- ci: add azure macOS + iOS build
- iOS: fix arm32 c++17 build(aligned allocate), disable private symbol texImageIOSurface
- free for gpl softwares


0.2.1 - 2019-04-28

- support win arm64 via clang, support llvm-rc
- AVGlue:
    - support user specified FFmpeg runtime libraries instead of built-in ones via environment var: AVUTIL_LIB, AVCODEC_LIB, AVFORMAT_LIB, AVFILTER_LIB, SWRESAMPLE_LIB
    - add FFmpeg decoder property "codec": glfwplay -c:v FFmpeg:codec=...
- Fix MFT p010 frame stride
- Fix cuda api

0.2.0 - 2019-04-01

- API:
  - add setBufferRange()
  - add/removeListener()=>onEvent()
  - add MediaInfo
  - add snapshot()
  - add setBackgroundColor()
- C ABI: should be stable
- New:
  - support dropping outdated packets for realtime streams
  - support fast accurate seek
  - auto rotate
  - switch decoder on the fly
  - first frame rendered event
- Fix m3u8 demux
- Fix a/v sync if audio thread exits earlier
- Fix incorrect buffering progress
- Fix undefined ndkmedia symbols at runtime and crash
- rpi: add native mmal video decoder(disabled now)
- rpi: fix eglimage cache
- rpi: start to adding omx
- MFT: check h264 constraints


0.1.0 - 2019-02-25

- ABI and API change. Single C/C++ SDK is compatible with any C++11 compiler and runtime. Library APIs moves into abi namespace.
- Hide more unnecessary symbols
- Simplify udio channel map
- Do not render audio if no audio device. crash fix
- Fix buffered bytes
- Implement CUDA decoder. Should be better than FFmpeg's CUVID.
- Use exported symbol `void* GetCurrentNativeDisplay()` in main to support vaapi/vdpau+x11egl interop


0.0.10 - 2019-01-28

- Individual audio tracks support.
- MFT decoder improve
    - crop visual area
    - decoded frame copy mode option
    - fix crashes
    - d3d11 0-copy rendering jitter fix
    - add audio decoders: aac, mp3 support
    - minimize IMFMediaBuffer copy
    - fix undefined MFCreateTrackedSample runtime error on win7
- P010, P016 rendering in OpenGL
- buffering event
- Apple: disable flat namespace
- unify decoder properties: "copy", "format"
- support win32 x86: api ptr type, float point compare
- MediaStatus changes
- fix android mediacodec arm64 lto link
- fix annexb packet filter(affects mediacodec)
- mediacodec: support yuv420p, support level 28 ndk apis and metadata, print decoder name
- vaapi: support info/error callback
- avcodec: fix lifetime of hwcontext(hwdec crash)
- gl: recreate texture before changing parameters to fix wrong display if hwdec=>swdec

0.0.9 - 2018-12-05

- dynamic load vdpau in GLVA module
- P010 for d3d11 decoded frames
- enable thin-lto for all
- fix crash if no egl dll
- fix iOS link error
- pulseaudio wip
- Audio backend refactor: async push backend
- exit dsound thread via event
- Add missing onOpen, onClose, onFlush in video decoders
- Add VideoToolbox(name is "VT") for macOS/iOS, support async mode. h265 hvcC, h264 avcC stream are supported, nal is not yet.
  Better performance than FFmpeg's videotoolbox.
  propertyes: threads, format, realTime, async
- Decode EOS only once to fix MediaCodec block in the end.
- Add MFT based video decoder for Win32(Vista+) and UWP. Supports h264, hevc, vp8/9 etc. Supports software decoding, dxva/d3d11 gpu acceleration.
- Add threads property for FFmpeg based decoders.
- Android: support posting messages from native code. try to load stl.
- WinRT/UWP: support IStorageFile


0.0.8 - 2018-10-07

- GL:
    - do not fill background color
    - fix wrong color if video changes
    - fix redraw in a new gl context
    - restore viewport, bo, shader program after rendering to support embedding in other frameworks
    - fix global vbo attributes changes
    - multi-thread improve
    - support NSWindow
    - fix resizing on macOS10.14
- player
    - Add volume, mute, playback rate, vide scale(including mirror) api
    - add getVideoFrame() to retreive currently rendered frame
    - surface type parameter for updateNativeWindow()
    - default preload policy change
    - render callback parameter to support multiple outputs
- fix gapless switch
- fix buffered duration
- apple: disable time pitch effect which results in inaccurate callback invoking time after AudioQueueReset
- playback rate is implemented in avfilter
- no sw fallback when opening hw decoder
- print build information
- fix old packet is enqueued after seek
- GLVA:
    - vaapi: fallback to drm interop if drm2 is not implemented
    - d3d11: always render mapped nv12 textures in rg format to fix es2 rendering(11 feature level 9.3)
- win32/winrt: observe surface size change via foreign window handle and less user code
- build: icf, clang-cl on windows, libc++ etc.
- test: add glfw, x11 example


0.0.7 - 2018-07-31

- support sunxi cedarv decoder and 0-copy rendering via UMP
- add mediacodec audio decoder
- unify mediacodec audio & video decoder
- improve mediacodec video decoder
- create decoder with options string
- buffering progress in duration
- drop frame less. control via env "VO_DROP"
- no MTA requirement for xaudio2
- player: support multiple renderers
- API: player.setAudioDecoders(...), tiled pixel format, seek flag
- build: improve linux cross build with lib++, support winrt via clang
- GLVA:
    - use native video buffer template except apple
    - vaapi + drm(prime) support
    - fix mapped texture reuse for multi-renderers rendering(except d3d)


0.0.6 - 2018-06-02

- support clang-cl host build and cross build for windows
- support opensource gcc on macOS
- improve Packet.buffer
- support rendering frame whose vertical stride > height
- fix and improve VideoFrame ctor
- fix seek block because of long time sleep
- add MediaStatus::PreparedMedia, fix mediaStatus() bugs
- frame boost options for FrameReader and player prepared callback
- fix wrong frame displayed in gapless switch via frame boost option
- Android AudioTrack support
- Android MediaCodec video decoder support, via ndk api implemented in ndk or jni (JMI+AND project)
- GLVA:
    - test D3D11 nv12 SR texture support
    - map to host support: d3d9, d3d11, vdpau
    - CUDA stream support and enabled by default
    - fix android context change check
    - fix MMAL pool is used for mesa vc4 environment
- AVGlue:
    - use new api avcodec_get_hw_frames_parameters
    - set texture flags via hwframesctx to enable d3d11va-eglstream 0-copy
    - improve Buffer and AVBufferRef, support offset
    - fix mkv packet decode error in ffmpeg4.0+
    - fix mediacodec because of ref leak
    - use CUDA stream shared from glva native buffer pool


0.0.5 - 2018-05-05

- video decoder supports glva via wrapper
- fix mapped host frame stride and format
- qr code overlay and license check module
- OpenSL use system stream type and the sound is louder
- ALSA: fix reopen error
- Support direct sound
- Fix rendering audio with invalid padding data
- GL: grab rendered frame, user shader api
- player: no currentMediaChanged in dtor to ensure accessing player object is safe
- FrameReader: improve AOT frame boost(no api now), improve state update and no more unexpected dead blocks
- Gapless switch improvements(mostly in audio)
- log is default disabled, set null handler behavior change
- build: cmake 3.5+ is required, ubsan, fix header install
- GLVA
    - api change: support multiple textures, no more map callback, frame size parameter
    - NativeVideoBuffer wrapper for c api
    - Fix texture reuse optimization(apple, d3d, mmal)
    - add VA-API interop with X11 Pixmap, for both  EGL and GLX
    - add VDPAU interop with X11 Pixmap, and GLX extension, for both  EGL and GLX
    - D3D9 interop with WGL works now
    - support D3D11 interop with WGL
    - D3D11 interop with EGLStream works now and it becomes the default
    - D3D11 supports texture as egl client buffer(requires ANGLE master)
    - fix potential EGLSurface leak in CVPixelBuffer interop
- AVGlue
    - improve linking static ffmpeg on windows
    - support VA-API, VDPAU
    - less deprecated apis
    - fix AVFrame leak in filter, use smart ptrs for av types
    - fix the case avpacket data is different from buf.data


0.0.4 - 2018-04-07

- requires cmake3, fix x64
- gnu stl is default for rpi
- works with vs2013 again(wgl only)
- frame drop api, no implementation
- support ALSA, it's default for linux
- improve audio renderer: flush, pause, delay, timestamp, unified, write
- remove audio renderer open/close apis. reopen internally when necessary
- improve a/v sync, support no active audio track
- opengl context local storage improvements: per object, custom creator and initializer etc.
- d3d11 interop with angle egl, for desktop and uwp
- fix teared content in d3d11 interop
- apple CVPixelBuffer interop with ANGLE EGL
- disable opengl shader cache sharing
- fix 16bit rendering in ES3
- videorenderer: fix 2d transform matrix(aspect ratio), optimize matrix uploading
- GLVA: refactor, unify and simplify context change handling
- support CUDA
- frame capture api without implementation
- avglue: new ffmpeg hwconfig apis, support nvdec, hw device ctx from us, hw copy decoder, mediacodec hwcontext
