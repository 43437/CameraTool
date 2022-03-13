#ifndef CCAMERASTREAMCLIENT_H
#define CCAMERASTREAMCLIENT_H

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
#include "sdlplayer/csdlplayer.h"
#include "cdecodehelper.h"
#include <string>

using std::string;
class CCameraStreamClient : public CSDLPlayer
{
public:
    CCameraStreamClient();
    void Start();

private:
    enum
    {
        ePixFmt = AV_PIX_FMT_YUV420P,
        eVideoSizeW = 1280,
        eVideoSizeH = 720,
    };

protected:
    virtual void InitFrameData();
    virtual void GetRenderData(unsigned char*& pBuffer, int& iFrameDataPitch);

private:
    void InitCodec();

private:
    AVFormatContext         *m_pAVFormatCtx;
    AVCodecContext          *m_pAVCodecCtx;
    const AVCodec           *m_pAVcodec;
    CDecodeHelper           *m_pDecodeHelper;
    string                  m_strURL;
    AVFrame                 *m_pFrame;
    uint8_t                 *m_pFrameBuff;
    int                     m_FrameBuffSize;
    AVPacket                m_stuPacket;
};

#endif // CCAMERASTREAMCLIENT_H
