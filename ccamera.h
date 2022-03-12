#ifndef CCAMERA_H
#define CCAMERA_H

#ifdef __cplusplus
extern "C"{
#endif
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libavdevice/avdevice.h>
  #include <libswscale/swscale.h>
  #include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif
#include <string>

using std::string;

class CCamera
{
public:
    CCamera();
    ~CCamera();
    void GetData(AVPacket* packet);
    void GetSize(int& width, int& height);
    AVPixelFormat GetPixFmt();

private:
    void Init();
    void Open();

private:
    AVFormatContext         *m_pFormatCtx;
    const AVInputFormat     *m_pInputFmt;
    AVCodecContext          *m_pCodecCtx;
    const AVCodec           *m_pCodec;
    string                  m_strInputFmt;
    string                  m_strInputFile;
    int                     m_iVideoIndex;
};

#endif // CCAMERA_H
