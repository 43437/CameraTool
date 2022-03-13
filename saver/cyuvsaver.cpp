#include "cyuvsaver.h"
#include <iostream>

CYUVSaver::CYUVSaver(CCamera& camera) : m_rCamera(camera)
{
    Init();
}

void CYUVSaver::Init()
{
    m_iWidth = 1280;
    m_iHeight = 720;
    m_ePixFmt = AV_PIX_FMT_YUV420P;
    m_strSaveFile = "out.yuv";
}

void CYUVSaver::Save()
{
    uint8_t     *srcData[4];
    uint8_t     *dstData[4];
    int         srcStride[4];
    int         dstStride[4];
    int         iSrcWidth = m_iWidth;
    int         iSrcHeight = m_iHeight;

    //Get YUV data
    AVPacket* packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    m_rCamera.GetSize(iSrcWidth, iSrcHeight);
    m_rCamera.GetData(packet);

    //Scale begin
    av_image_alloc(srcData, srcStride, iSrcWidth, iSrcHeight, m_rCamera.GetPixFmt(), 16);
    int dstSize = av_image_alloc(dstData, dstStride, m_iWidth, m_iHeight, m_ePixFmt, 16);
    memcpy(srcData[0], packet->data, packet->size);

    SwsContext* swsCtx = sws_getContext(iSrcWidth, iSrcHeight, m_rCamera.GetPixFmt(), m_iWidth, m_iHeight, m_ePixFmt, SWS_BILINEAR, NULL, NULL, NULL);
    sws_scale(swsCtx, srcData, srcStride, 0, iSrcHeight, dstData, dstStride);
    //Scale end

    //Save YUV
    FILE* fp = std::fopen(m_strSaveFile.c_str(), "wb");
    if (fp == nullptr)
    {
        std::cout << "open out file failed, now return" << std::endl;
        return;
    }
    std::fwrite(dstData[0], 1, dstSize, fp);
}
