#ifndef CCAMERASOCKCLIENT_H
#define CCAMERASOCKCLIENT_H

#ifdef __cplusplus
extern "C"{
#endif
#include <sys/socket.h>
#include <arpa/inet.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif
#include "sdlplayer/csdlplayer.h"
#include <string>
#include "cdecodehelper.h"

using std::string;
class CCameraSockClient : public CSDLPlayer
{
public:
    CCameraSockClient();
    void Start();

protected:
    virtual void InitFrameData();
    virtual void GetRenderData(unsigned char*& pBuffer, int& iFrameDataPitch);

private:
    enum
    {
        eSendBufSize = 1024000,
        ePixFmt = AV_PIX_FMT_YUV420P,
        eVideoSizeW = 1280,
        eVideoSizeH = 720,
    };

private:
    bool OpenConnection();
    void InitCodec();
    void Receive();

private:
    int                     m_iSockConn;
    string                  m_arrRemoteAddr;
    int                     m_iRemotePort;
    AVCodecContext          *m_pAVCodecCtx;
    const AVCodec           *m_pAVcodec;
    CDecodeHelper           *m_pDecodeHelper;
    AVFrame                 *m_pFrame;
    uint8_t                 *m_pFrameBuff;
    int                     m_FrameBuffSize;
};

#endif // CCAMERASOCKCLIENT_H
