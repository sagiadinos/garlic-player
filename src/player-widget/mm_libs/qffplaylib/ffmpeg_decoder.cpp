#include "ffmpeg_decoder.h"

QtFFmpegPlayer::Decoder::Decoder(QObject *parent) : QThread(parent)
{
    //initializing all the containers, codecs and protocols
    pFormatContext = avformat_alloc_context();
}

QtFFmpegPlayer::Decoder::~Decoder()
{
    avformat_close_input(&pFormatContext);
    avformat_free_context(pFormatContext);
}

void QtFFmpegPlayer::Decoder::deinit()
{

    av_packet_free(&pPacket);
    av_frame_free(&pFrame);
    avcodec_close(pCodecContext);
    avcodec_free_context(&pCodecContext);
}

bool QtFFmpegPlayer::Decoder::init()
{
    if (!pFormatContext)
    {
        addErrorText("ERROR could not allocate memory for Format Context");
        return false;
    }

    return true;
}

bool QtFFmpegPlayer::Decoder::load(QString fp)
{
    file_path = fp;
    if (fp.isEmpty())
    {
        addErrorText("filepath is empty");
        return false;
    }

    if (avformat_open_input(&pFormatContext, fp.toStdString().c_str(), NULL, NULL) != 0)
    {
        addErrorText("ERROR could not open the file");
        return false;
    }
    return true;
}



bool QtFFmpegPlayer::Decoder::findStreams()
{
    if (avformat_find_stream_info(pFormatContext,  NULL) < 0)
    {
        addErrorText("ERROR could not get the stream info");
        return false;
    }

    video_stream_index    = av_find_best_stream(pFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    VideoCodecParameters  = pFormatContext->streams[video_stream_index]->codecpar;
    VideoCodec            = avcodec_find_decoder(VideoCodecParameters->codec_id);

 //   audio_stream_index    = av_find_best_stream(pFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
 //   AudioCodecParameters  = pFormatContext->streams[audio_stream_index]->codecpar;
 //   AudioCodec            = avcodec_find_decoder(AudioCodecParameters->codec_id);

    if (video_stream_index == -1 && audio_stream_index == -1)
    {
       addErrorText("File " + file_path + " does not contain neither a video nor an audio stream!");
       return false;
    }

     return true;
}

bool QtFFmpegPlayer::Decoder::prepareDecode()
{
    pCodecContext = avcodec_alloc_context3(VideoCodec);
    if (!pCodecContext)
    {
        addErrorText("failed to allocated memory for AVCodecContext");
        return false;
    }

    int error = avcodec_parameters_to_context(pCodecContext, VideoCodecParameters);
    // Fill the codec context based on the values from the supplied codec parameters
    if (error < 0)
    {
        addErrorText("failed to copy codec params to codec context");
        return false;
    }

    error = avcodec_open2(pCodecContext, VideoCodec, NULL);
    // Initialize the AVCodecContext to use the given AVCodec.
    if (error < 0)
    {
        addErrorText("failed to open codec through avcodec_open2");
        return false;
    }

    pFrame = av_frame_alloc();
    if (!pFrame)
    {
        addErrorText("failed to allocate memory for AVFrame");
        return false;
    }

    pPacket = av_packet_alloc();
    if (!pPacket)
    {
        addErrorText("failed to allocate memory for AVPacket");
        return false;
    }

    return true;
}

void QtFFmpegPlayer::Decoder::run()
{

    int response   = 0;
    int frame_rate = 0;

    // fill the Packet with data from the Stream
    while (av_read_frame(pFormatContext, pPacket) >= 0)
    {
        if (pPacket->stream_index == video_stream_index)
        {
            response = decodePacket();
            if (response < 0)
                break;
            // Calculate the Frame rate to determine the wait until next frame can be decoded
            // If 25 fps, then we nee to wait 40ms untill next frame can be decoded
            AVStream *stream = pFormatContext->streams[video_stream_index];
            frame_rate = stream->avg_frame_rate.num/stream->avg_frame_rate.den;
            if (frame_rate == 0)
                frame_rate = 25;

            delay(1000/frame_rate);
        }
        av_packet_unref(pPacket);
        av_packet_free(&pPacket);
        pPacket = av_packet_alloc();
    }
    emit endOfFile();

}

void QtFFmpegPlayer::Decoder::setRenderer(VideoOut *r)
{
    MyVideoOut = r;
}

QString QtFFmpegPlayer::Decoder::getErrorText()
{
    return error_text;
}


int QtFFmpegPlayer::Decoder::decodePacket()
{
    // Supply raw packet data as input to a decoder
    int response = avcodec_send_packet(pCodecContext, pPacket);
    if (response < 0)
    {
        qWarning("Error while sending a packet to the decoder: %s", av_err2str(response));
        return response;
    }

    while (response >= 0)
    {
        pFrame = av_frame_alloc();
        // Return decoded output data (into a frame) from a decoder
        response = avcodec_receive_frame(pCodecContext, pFrame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
        {
            av_frame_unref(pFrame);
            break;
        }
        else if (response < 0)
        {
            qWarning("Error while receiving a frame from the decoder: %s", av_err2str(response));
            av_frame_unref(pFrame);
            return response;
        }

        if (response >= 0)
        {
            MyVideoOut->placeDecodedImage(convertFrameToImage());
        }
        av_frame_unref(pFrame);
        av_frame_free(&pFrame);
    }
    return 0;
}

QImage QtFFmpegPlayer::Decoder::convertFrameToImage()
{
    AVPixelFormat pixFormat;
    bool changeColorspaceDetails = false;
    switch (pCodecContext->pix_fmt)
      {
        case AV_PIX_FMT_YUVJ420P:
          pixFormat = AV_PIX_FMT_YUV420P;
          changeColorspaceDetails = true;
          break;
        case AV_PIX_FMT_YUVJ422P:
          pixFormat = AV_PIX_FMT_YUV422P;
          changeColorspaceDetails = true;
          break;
        case AV_PIX_FMT_YUVJ444P:
          pixFormat = AV_PIX_FMT_YUV444P;
          changeColorspaceDetails = true;
          break;
        case AV_PIX_FMT_YUVJ440P:
          pixFormat = AV_PIX_FMT_YUV440P;
          changeColorspaceDetails = true;
          break;
        default:
          pixFormat = pCodecContext->pix_fmt;
      }

    SwsContext* img_convert_ctx = sws_getContext(pFrame->width, pFrame->height, pixFormat,
                                     pFrame->width, pFrame->height,
                                     AV_PIX_FMT_RGB24,
                                     SWS_BICUBIC, NULL, NULL, NULL);

    if(!img_convert_ctx)
    {
        qDebug() << "Failed to create sws context";
        return QImage();
    }

/*    if (changeColorspaceDetails)
    {
        // change the range of input data by first reading the current color space and then setting it's range as yuvj.
        int dummy[4];
        int srcRange, dstRange;
        int brightness, contrast, saturation;
        sws_getColorspaceDetails(img_convert_ctx, (int**)&dummy, &srcRange, (int**)&dummy, &dstRange, &brightness, &contrast, &saturation);
        const int* coefs = sws_getCoefficients(SWS_CS_DEFAULT);
        srcRange = 1; // this marks that values are according to yuvj
        sws_setColorspaceDetails(img_convert_ctx, coefs, srcRange, coefs, dstRange, brightness, contrast, saturation);
        delete coefs;
    }
*/
    // prepare line sizes structure as sws_scale expects
    int rgb_linesizes[8] = {0};
    rgb_linesizes[0]     = 3*pFrame->width;

    // prepare char buffer in array, as sws_scale expects
    unsigned char* rgbData[8];
    int imgBytesSyze = 3*pFrame->height*pFrame->width;

    // as explained above, we need to alloc extra 64 bytes
  //  rgbData[0] = new unsigned char[imgBytesSyze+64];
    rgbData[0] = (unsigned char *)malloc(imgBytesSyze+64);

    if(!rgbData[0])
    {
        qWarning() << "Error allocating buffer for frame conversion";
        free(rgbData[0]);
        sws_freeContext(img_convert_ctx);
        return QImage();
    }

    if(sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0, pFrame->height, rgbData, rgb_linesizes) != pFrame->height)
    {
        qWarning() << "Error changing frame color range";
        free(rgbData[0]);
        sws_freeContext(img_convert_ctx);
        return QImage();
    }

    // then create QImage and copy converted frame data into it
    QImage image(pFrame->width, pFrame->height, QImage::Format_RGB888);
    for(int y = 0; y < pFrame->height; y++)
    {
        memcpy(image.scanLine(y), rgbData[0] + y*3*pFrame->width, 3*pFrame->width);
    }

    free(rgbData[0]);
    sws_freeContext(img_convert_ctx);

    return image;
}

void QtFFmpegPlayer::Decoder::addErrorText(QString error)
{
    error_text = error;
}

void QtFFmpegPlayer::Decoder::delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
