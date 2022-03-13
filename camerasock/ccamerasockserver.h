#ifndef CCameraSockServer_H
#define CCameraSockServer_H

#include "camera/ccamerayuv.h"
#ifdef __cplusplus
extern "C"{
#endif
#include <sys/socket.h>
#include <arpa/inet.h>
#ifdef __cplusplus
};
#endif

class CCameraSockServer
{
public:
    CCameraSockServer();
    void Start();

private:
    enum
    {
        eSendBufSize = 1024000,
        ePixFmt = AV_PIX_FMT_YUV420P,
        eVideoSizeW = 1280,
        eVideoSizeH = 720,
    };

private:
    void InitCodec();
    void Init();
    void Server();
    void Publish(AVPacket& stuOutPkt);

private:
    CCameraYUV              m_objCameraYUV;
    uint8_t                 m_arrSendBuf[eSendBufSize];
    string                  m_arrLocalAddr;
    int                     m_iLocalPort;
    int                     m_iSockConn;
    uint8_t                 *m_pFrameBuff;
    AVFrame                 *m_pFrame;
    AVCodecContext          *m_pAVCodecCtx;
    const AVCodec           *m_pAVcodec;
};

#endif // CCameraSockServer_H
