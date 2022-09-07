#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include <QObject>
#include <QDebug>
#include <QLabel>
#include <QTime>
#include <QThread>
#include <QCoreApplication>
#include <QTimer>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#include "videoout.h"


namespace QtFFmpegPlayer
{
    class Decoder : public QThread
    {
        Q_OBJECT
    public:
        explicit Decoder(QObject *parent = nullptr);
        ~Decoder();
        void deinit();
        bool init();
        bool load (QString fp);
        bool findStreams();
        bool prepareDecode();
        void run();
        void setRenderer(VideoOut *r);
        QString getErrorText();
    private:
        QString            file_path;
        QString            error_text;
        int                video_stream_index = -1;
        int                audio_stream_index = -1;
        VideoOut          *MyVideoOut;
        QPixmap            loaded_image;
        AVFormatContext   *pFormatContext       = Q_NULLPTR;
        AVCodecContext    *pCodecContext        = Q_NULLPTR;
        AVFrame           *pFrame               = Q_NULLPTR;
        AVPacket          *pPacket              = Q_NULLPTR;
        const AVCodec     *VideoCodec           = Q_NULLPTR;
        const AVCodec     *AudioCodec           = Q_NULLPTR;
        AVCodecParameters *VideoCodecParameters = Q_NULLPTR;
        AVCodecParameters *AudioCodecParameters = Q_NULLPTR;


        QImage convertFrameToImage();
        void addErrorText(QString error);
        int decodePacket();
        void fillRenderer(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);
        void delay(int msec);
    signals:
        void endOfFile();
    };
}
#endif // FFMPEGDECODER_H
