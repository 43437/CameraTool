#ifndef CCAMERAPLAYER_H
#define CCAMERAPLAYER_H

#include "csdlplayer.h"
#include "ccamera.h"

class CCameraPlayer : public CSDLPlayer
{
public:
    CCameraPlayer(CCamera& rCamera);

protected:
    virtual void InitFrameData();
    virtual void GetRenderData(unsigned char*& pBuffer, int& iFrameDataPitch);

private:
    CCamera             &m_rCamera;
    SwsContext          *m_ConvertCtx;
    AVFrame             *m_pFrameYUV;
    uint8_t             *m_pBufferYUV;
    int                 m_iBufferSizeYUV;
    uint8_t             *m_SrcData[4];
    uint8_t             *m_DstData[4];
    int                 m_SrcStride[4];
    int                 m_DstStride[4];
    AVPacket            m_stuPacket;
};

#endif // CCAMERAPLAYER_H
