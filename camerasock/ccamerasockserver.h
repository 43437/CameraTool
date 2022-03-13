#ifndef CCameraSockServer_H
#define CCameraSockServer_H

#include "ccamera.h"
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
    CCameraSockServer(CCamera& rCamera);
    void Start();

private:
    enum
    {
        eSendBufSize = 1024000,
    };

private:
    void Init();
    void Server();
    void Publish(AVPacket& stuOutPkt);

private:
    CCamera             &m_rCamera;
    uint8_t             m_arrSendBuf[eSendBufSize];
    string              m_arrLocalAddr;
    int                 m_iLocalPort;
    int                 m_iSockConn;
};

#endif // CCameraSockServer_H
