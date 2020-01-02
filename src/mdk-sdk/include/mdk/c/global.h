/*
 * Copyright (c) 2019 WangBin <wbsecg1 at gmail.com>
 * This file is part of MDK
 * MDK SDK: https://sourceforge.net/projects/mdk-sdk/files
 * Free for GPL softwares or non-commercial use.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 */
#pragma once
#include <inttypes.h>
#include <limits.h>

#if defined(_WIN32)
#define MDK_EXPORT __declspec(dllexport)
#define MDK_IMPORT __declspec(dllimport)
#else
#define MDK_EXPORT __attribute__((visibility("default")))
#define MDK_IMPORT __attribute__((visibility("default")))
#endif

#ifdef BUILD_MDK_STATIC
# define MDK_API
#else
# if defined(BUILD_MDK_LIB)
#  define MDK_API MDK_EXPORT
# else
#  define MDK_API MDK_IMPORT
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MDK_CALL(p, FN, ...) p->FN(p->object, ##__VA_ARGS__)

/*!
  \brief CallbackToken
  A callback can be registered by (member)function onXXX(obj, callback, CallbackToken* token = nullptr). With the returned token we can remove the callback by onXXX(nullptr, token).
  Non-null callback(.opaque != null): register a callback and return a token(if not null).
  Null callback(.opaque == null) + non-null token: can remove then callback of given token.
  Null callback(.opaque == null) + null token: clear all callbacks.
 */
typedef uint64_t MDK_CallbackToken;

typedef enum MDK_MediaType {
    MDK_MediaType_Unknown = -1,
    MDK_MediaType_Video = 0,
    MDK_MediaType_Audio = 1,
    MDK_MediaType_Subtitle = 3,
} MDK_MediaType;

/*!
  \brief The MediaStatus enum
  Defines the io status of a media stream,
  Use flags_added/removed() to check the change, for example buffering after seek is Loaded|Prepared|Buffering, and changes to Loaded|Prepared|Buffered when seek completed
 */
typedef enum MDK_MediaStatus
{
    MDK_MediaStatus_NoMedia = 0, /* initial status, not invalid. // what if set an empty url and closed?*/
    MDK_MediaStatus_Unloaded = 1, /* unloaded // (TODO: or when a source(url) is set?)*/
    MDK_MediaStatus_Loading = 1<<1, /* when source is set*/
    MDK_MediaStatus_Loaded = 1<<2, /* if auto load and source is set. player is stopped state*/
    MDK_MediaStatus_Prepared = 1<<8, /* all tracks are buffered and ready to decode frames. tracks failed to open decoder are ignored*/
    MDK_MediaStatus_Stalled = 1<<3, /* insufficient buffering or other interruptions (timeout, user interrupt)*/
    MDK_MediaStatus_Buffering = 1<<4, /* NOT IMPLEMENTED*/
    MDK_MediaStatus_Buffered = 1<<5, /* when playing //NOT IMPLEMENTED*/
    MDK_MediaStatus_End = 1<<6, /* Playback has reached the end of the current media.*/
    MDK_MediaStatus_Seeking = 1<<7, /* can be used with Buffering, Loaded. FIXME: NOT IMPLEMENTED*/
    MDK_MediaStatus_Invalid = 1<<31, /*  invalid media source*/
} MDK_MediaStatus;

typedef struct mdkMediaStatusChangedCallback {
    bool (*cb)(MDK_MediaStatus, void* opaque);
    void* opaque;
} mdkMediaStatusChangedCallback;

/*!
  \brief The State enum
  Current playback state. Set/Get by user
 */
typedef enum MDK_State {
    MDK_State_NotRunning,
    MDK_State_Stopped = MDK_State_NotRunning,
    MDK_State_Running,
    MDK_State_Playing = MDK_State_Running, /* start/resume to play*/
    MDK_State_Paused,
} MDK_State;
typedef MDK_State MDK_PlaybackState;

typedef struct mdkStateChangedCallback {
    void (*cb)(MDK_State, void* opaque);
    void* opaque;
} mdkStateChangedCallback;

typedef enum MDKSeekFlag {
    /* choose one of SeekFromX */
    MDK_SeekFlag_From0       = 1,    /* relative to time 0*/
    MDK_SeekFlag_FromStart   = 1<<1, /* relative to media start position*/
    MDK_SeekFlag_FromNow     = 1<<2, /* relative to current position, the seek position can be negative*/
    MDK_SeekFlag_Byte        = 1<<5,
    /* combine the above values with one of the following*/
    MDK_SeekFlag_KeyFrame    = 1<<8, /* fast key-frame seek, forward if Backward is not set. If not set, it's accurate seek but slow, implies backward seek internally*/
    MDK_SeekFlag_Fast        = MDK_SeekFlag_KeyFrame,
    MDK_SeekFlag_AnyFrame    = 1<<9, /* fast, broken image if video format has key frames. TODO: remove?*/

    /* Useful if seek backward repeatly, .i.e. target < playback(not buffered) position. result positions may be the same repeatly if seek forward w/ this flag, or seek backward w/o this flag*/
    MDK_SeekFlag_Backward    = 1<<16, /* for KeyFrame seek only. NOTE: FrameReader/PacketIO only. It has no effect to (un)set this flag in MediaControl/MediaPlayer and higher level apis*/
    MDK_SeekFlag_Default     = MDK_SeekFlag_KeyFrame|MDK_SeekFlag_FromStart
} MDK_SeekFlag;

/*!
  \brief javaVM
  Set/Get current java vm
  \param vm null to get current vm
  \return vm before set
 */
MDK_API void* MDK_javaVM(void* vm);

typedef enum MDK_LogLevel {
    MDK_LogLevel_Off,
    MDK_LogLevel_Error,
    MDK_LogLevel_Warning,
    MDK_LogLevel_Info,
    MDK_LogLevel_Debug,
    MDK_LogLevel_All
} MDK_LogLevel;
MDK_API void MDK_setLogLevel(MDK_LogLevel value);
MDK_API MDK_LogLevel MDK_logLevel();
/* \brief setLogHandler
  if log handler is not set, i.e. setLogHandler() was not called, log is disabled.
  if set to non-null handler, log will be passed to the handler.
  if previous handler is set by user and not null, then call setLogHandler(nullptr) will print to stderr, and call setLogHandler(nullptr) again to silence the log
*/
typedef struct mdkLogHandler {
    void (*cb)(MDK_LogLevel, const char*, void* opaque);
    void* opaque;
} mdkLogHandler;
MDK_API void MDK_setLogHandler(mdkLogHandler);

/*
 keys:
 - path to ffmpeg runtime libraries: avutil_lib, avcodec_lib, avformat_lib, swresample_lib, avfilter_lib
*/
MDK_API void MDK_setGlobalOptionString(const char* key, const char* value);
MDK_API void MDK_setGlobalOptionInt32(const char* key, int value);
/*
  events:
  timestamp(ms) + "render.video" + "1st_frame"
  "decoder.audio/video" + "open" + stream: decoder of a stream is open, or failed to open if error != 0. TODO: do not use "open"?
  error + "reader.buffering": error is buffering progress
  error + "thread.audio/video" + stream: decoder thread is started (error = 1) and about to exit(error = 0)
*/
typedef struct mdkMediaEvent {
    int64_t error; /* result <0: error code(fourcc?). >=0: special value depending on event*/
    const char* category;
    const char* detail; /* if error, detail can be error string*/

    union {
        struct {
            int stream;
        } decoder;
    };
} mdkMediaEvent;

/*
bool MDK_SomeFunc(SomeStruct*, mdkStringMapEntry* entry)
entry: in/out, can not be null.
Input entry->priv is null: 
The result entry points to the first entry containing the same key as entry->key, or the first entry if entry->key is null.
The result entry->priv is set to a new value by api. 
Input entry->priv is not null(set by the api): the result entry points to the next entry.
return: true if entry is found, false if not. 
*/
typedef struct mdkStringMapEntry {
    const char* key;    /* input: set by user to query .value field if priv is null
                           output: set by api if priv is not null (set by api) */
    const char* value;  /* output: set by api, or not touched if no such key */

    void* priv; /* input/output: set by api */
} mdkStringMapEntry;

#ifdef __cplusplus
}
#endif
