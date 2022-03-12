#ifndef CVIDEOSAVER_H
#define CVIDEOSAVER_H

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
#include "ccamera.h"

class CVideoSaver
{
public:
    CVideoSaver(CCamera& camera);
    void Save();

private:
    void Init();

private:
    CCamera&                m_rCamera;
    AVFormatContext         *m_pFormatCtx;
    AVCodecContext          *m_pAVCodecCtx;
    const AVCodec           *m_pAVcodec;
    string                  m_strSaveFile;
    int                     m_iVideoIndex;
    int                     m_iSaveWidth;
    int                     m_iSaveHeight;
    AVPixelFormat           m_eDstPixFmt = AV_PIX_FMT_YUV420P;
};

#endif // CVIDEOSAVER_H
