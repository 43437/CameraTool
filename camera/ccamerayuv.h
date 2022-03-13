#ifndef CCAMERAYUV_H
#define CCAMERAYUV_H

#include "camera/ccamera.h"

class CCameraYUV : public CCamera
{
public:
    CCameraYUV(int iSizeW, int iSizeH, AVPixelFormat ePixFmt);
    void GetData(uint8_t* buf);
    int GetDataSize();

private:
    void Init();

private:
    SwsContext*         m_pSwsCtx;
    int                 m_iCameraW;
    int                 m_iCameraH;
    int                 m_iSizeW;
    int                 m_iSizeH;
    AVPixelFormat       m_ePixFmt;
    uint8_t             *m_pSrcData[4];
    uint8_t             *m_pDstData[4];
    int                 m_arrSrcStride[4];
    int                 m_arrDstStride[4];
    AVPacket            stuPacket;
    int                 m_DstBufSize;
};

#endif // CCAMERAYUV_H
