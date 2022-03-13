#ifndef CCAMERASTREAMSERVER_H
#define CCAMERASTREAMSERVER_H

#include "camera/ccamerayuv.h"

class CCameraStreamServer
{
public:
    CCameraStreamServer();
    void Start();

private:
    enum
    {
        ePixFmt = AV_PIX_FMT_YUV420P,
        eVideoSizeW = 1280,
        eVideoSizeH = 720,
    };

private:
    void Init();
    void InitDataBuf();
    void InitCodec();

private:
    CCameraYUV              m_objCameraYUV;
    AVCodecContext          *m_pAVCodecCtx;
    const AVCodec           *m_pAVcodec;
    AVFormatContext         *m_pAVFmtCtx;
    uint8_t                 *m_pFrameBuff;
    AVFrame                 *m_pFrame;
    string                  m_strURL;
};

#endif // CCAMERASTREAMSERVER_H
