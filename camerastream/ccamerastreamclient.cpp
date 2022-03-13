#include "ccamerastreamclient.h"
#include "cdecodehelper.h"
#include <iostream>
#include <cassert>

CCameraStreamClient::CCameraStreamClient() : CSDLPlayer()
  ,m_pAVFormatCtx(nullptr)
  ,m_pAVCodecCtx(nullptr)
  ,m_pAVcodec(nullptr)
  ,m_pDecodeHelper(nullptr)
  ,m_strURL("udp://127.0.0.1:8989")
  ,m_pFrame(nullptr)
  ,m_pFrameBuff(nullptr)
  ,m_FrameBuffSize(0)
{

}

void CCameraStreamClient::InitCodec()
{
    m_pAVFormatCtx = avformat_alloc_context();
    assert(nullptr != m_pAVFormatCtx);
    if (avformat_open_input(&m_pAVFormatCtx, m_strURL.c_str(), nullptr, nullptr) != 0)
    {
        std::cout << "open input failed. "<< std::endl;
        return;
    }

    if (avformat_find_stream_info(m_pAVFormatCtx, 0) < 0)
    {
        std::cout<<"find stream failed. "<<std::endl;
        return;
    }

    int videoStream =0;
    for (int i = 0; i < m_pAVFormatCtx->nb_streams; ++i)
    {
        if (AVMEDIA_TYPE_VIDEO == m_pAVFormatCtx->streams[i]->codecpar->codec_type)
        {
            videoStream = i;
            break;
        }
    }
    m_pAVcodec = avcodec_find_decoder(m_pAVFormatCtx->streams[videoStream]->codecpar->codec_id);
    assert(nullptr != m_pAVcodec);
    m_pAVCodecCtx = avcodec_alloc_context3(m_pAVcodec);
    assert(nullptr != m_pAVCodecCtx);
    avcodec_parameters_to_context(m_pAVCodecCtx, m_pAVFormatCtx->streams[videoStream]->codecpar);

    m_pDecodeHelper = new CDecodeHelper(m_pAVCodecCtx);
}

void CCameraStreamClient::InitFrameData()
{
    m_PixelW = eVideoSizeW;
    m_PixelH = eVideoSizeH;
    m_Pixformat = SDL_PIXELFORMAT_IYUV;

    InitCodec();

    m_FrameBuffSize = av_image_get_buffer_size((AVPixelFormat)ePixFmt, eVideoSizeW, eVideoSizeH, 16);
    m_pFrameBuff = (unsigned char *)av_malloc(m_FrameBuffSize);
    m_pFrame = av_frame_alloc();
    memset(&m_stuPacket, 0, sizeof(AVPacket));
}

void CCameraStreamClient::GetRenderData(unsigned char*& pBuffer, int& iFrameDataPitch)
{
    pBuffer = nullptr;
    iFrameDataPitch = 0;

    av_image_copy_to_buffer(m_pFrameBuff, m_FrameBuffSize,
            m_pFrame->data, m_pFrame->linesize,
            (AVPixelFormat)ePixFmt,
            eVideoSizeW, eVideoSizeH, 16);

    av_read_frame(m_pAVFormatCtx, &m_stuPacket);
    if (m_pDecodeHelper->Decode(&m_stuPacket, m_pFrame))
    {
        pBuffer = m_pFrameBuff;
        iFrameDataPitch = eVideoSizeW;
    }
}

void CCameraStreamClient::Start()
{

}
