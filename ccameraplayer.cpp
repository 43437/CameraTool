#include "ccameraplayer.h"

CCameraPlayer::CCameraPlayer(CCamera& rCamera) : CSDLPlayer()
  ,m_rCamera(rCamera)
  ,m_ConvertCtx(nullptr)
  ,m_pFrameYUV(nullptr)
  ,m_pBufferYUV(nullptr)
{

}

void CCameraPlayer::InitFrameData()
{
    m_WindowW = 1280;
    m_WindowH = 720;
    m_Pixformat = SDL_PIXELFORMAT_IYUV;
    m_rCamera.GetSize(m_PixelW, m_PixelH);

    int iDstWidth = m_PixelW;
    int iDstHeight = m_PixelH;
    av_image_alloc(m_SrcData, m_SrcStride, m_PixelW, m_PixelH, m_rCamera.GetPixFmt(), 16);
    m_iBufferSizeYUV = av_image_alloc(m_DstData, m_DstStride, iDstWidth, iDstHeight, AV_PIX_FMT_YUV420P, 16);

    m_pFrameYUV = av_frame_alloc();
    m_pBufferYUV = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,  iDstWidth, iDstHeight, 16));
    av_image_fill_arrays(m_pFrameYUV->data, m_pFrameYUV->linesize, m_pBufferYUV, AV_PIX_FMT_YUV420P, iDstWidth, iDstHeight, 16);

    m_ConvertCtx = sws_getContext(m_PixelW, m_PixelH, m_rCamera.GetPixFmt(), iDstWidth, iDstHeight, AV_PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);
}

void CCameraPlayer::GetRenderData(unsigned char*& pBuffer, int& iFrameDataPitch)
{
    m_rCamera.GetData(&m_stuPacket);

    //Scale begin
    memcpy(m_SrcData[0], m_stuPacket.data, m_stuPacket.size);

    //scale the input frame to ouput frame
    sws_scale(m_ConvertCtx, m_SrcData, m_SrcStride, 0, m_PixelH, m_DstData, m_DstStride);

    //when copy data to pFrameBuff, that means the data fill in m_pFrameYUV
    memcpy(m_pBufferYUV, m_DstData[0], m_iBufferSizeYUV);
    //Scale end

    pBuffer = m_pFrameYUV->data[0];
    iFrameDataPitch = m_pFrameYUV->linesize[0];
}
