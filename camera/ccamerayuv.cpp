#include "ccamerayuv.h"

CCameraYUV::CCameraYUV(int iSizeW, int iSizeH, AVPixelFormat ePixFmt) : CCamera()
  ,m_pSwsCtx(nullptr)
  ,m_iSizeW(iSizeW)
  ,m_iSizeH(iSizeH)
  ,m_ePixFmt(ePixFmt)
{
    Init();
}

void CCameraYUV::Init()
{
    GetSize(m_iCameraW, m_iCameraH);
    m_pSwsCtx = sws_getContext(m_iCameraW, m_iCameraH, GetPixFmt(), m_iSizeW, m_iSizeH, m_ePixFmt, SWS_BILINEAR, NULL, NULL, NULL);

    av_image_alloc(m_pSrcData, m_arrSrcStride, m_iCameraW, m_iCameraH, GetPixFmt(), 16);
    m_DstBufSize = av_image_alloc(m_pDstData, m_arrDstStride, m_iSizeW, m_iSizeH, m_ePixFmt, 16);
}

int CCameraYUV::GetDataSize()
{
    return m_DstBufSize;
}

void CCameraYUV::GetData(uint8_t* buf)
{
    CCamera::GetData(&stuPacket);
    memcpy(m_pSrcData[0], stuPacket.data, stuPacket.size);
    sws_scale(m_pSwsCtx, m_pSrcData, m_arrSrcStride, 0, m_iCameraH, m_pDstData, m_arrDstStride);
    memcpy(buf, m_pDstData[0], m_DstBufSize);
}
