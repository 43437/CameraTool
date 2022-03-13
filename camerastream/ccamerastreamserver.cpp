#include "ccamerastreamserver.h"
#include <iostream>
#include <cassert>
#include "cencodhelper.h"

CCameraStreamServer::CCameraStreamServer() : m_objCameraYUV(eVideoSizeW, eVideoSizeH, (AVPixelFormat)ePixFmt)
  ,m_pAVCodecCtx(nullptr)
  ,m_pAVcodec(nullptr)
  ,m_pAVFmtCtx(nullptr)
  ,m_pFrameBuff(nullptr)
  ,m_pFrame(nullptr)
  ,m_strURL("udp://127.0.0.1:8989")
{
    Init();
}

void CCameraStreamServer::Init()
{
    InitDataBuf();
    InitCodec();
}

void CCameraStreamServer::InitDataBuf()
{
    int iDataSize = m_objCameraYUV.GetDataSize();
    m_pFrame = av_frame_alloc();
    m_pFrame->width = eVideoSizeW;
    m_pFrame->height = eVideoSizeH;
    m_pFrame->format = (AVPixelFormat)ePixFmt;
    m_pFrameBuff = (uint8_t *)av_malloc(iDataSize);
    av_image_fill_arrays(m_pFrame->data, m_pFrame->linesize, m_pFrameBuff, (AVPixelFormat)ePixFmt, eVideoSizeW, eVideoSizeH, 16);
}

void CCameraStreamServer::InitCodec()
{
    const AVOutputFormat *pMuxOutFmt = av_guess_format("mpegts", NULL, NULL);
    assert(nullptr != pMuxOutFmt);
    m_pAVcodec = avcodec_find_encoder(pMuxOutFmt->video_codec);
    assert(nullptr != m_pAVcodec);

    m_pAVFmtCtx = avformat_alloc_context();
    if (avio_open(&m_pAVFmtCtx->pb, m_strURL.c_str(), AVIO_FLAG_WRITE) < 0)
    {
        std::cout << "udp io open failed. " << std::endl;
        return;
    }
    m_pAVFmtCtx->oformat = pMuxOutFmt;

    AVStream *pMuxStream = avformat_new_stream(m_pAVFmtCtx, nullptr);
    assert(pMuxStream != NULL);
    m_pAVCodecCtx = avcodec_alloc_context3(m_pAVcodec);
    m_pAVCodecCtx->codec_id = pMuxOutFmt->video_codec;
    m_pAVCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    m_pAVCodecCtx->pix_fmt = (AVPixelFormat)ePixFmt;
    m_pAVCodecCtx->width = eVideoSizeW;
    m_pAVCodecCtx->height = eVideoSizeH;
    m_pAVCodecCtx->time_base.num =1;
    m_pAVCodecCtx->time_base.den = 25;
    m_pAVCodecCtx->bit_rate = 400000;
    m_pAVCodecCtx->gop_size=250;
    m_pAVCodecCtx->qmin = 10;
    m_pAVCodecCtx->qmax = 51;

    if (avcodec_open2(m_pAVCodecCtx, m_pAVcodec, nullptr) < 0 )
    {
        std::cout << "mux codec open failed." << std::endl;
        return;
    }
}

void CCameraStreamServer::Start()
{
    CEncodeHelper objEncodeHelper(m_pAVCodecCtx);
    AVPacket stuPacket;
    avformat_write_header(m_pAVFmtCtx, NULL);

    while (true)
    {
        m_objCameraYUV.GetData(m_pFrameBuff);
        if (objEncodeHelper.Encode(m_pFrame, &stuPacket))
        {
            av_interleaved_write_frame(m_pAVFmtCtx, &stuPacket);
        }
        else
        {
            std::cout << "objEncodeHelper.Encode failed." << std::endl;
        }
    }

    av_write_trailer(m_pAVFmtCtx);
}
