#include "ccamera.h"
#include <iostream>

CCamera::CCamera() : m_pFormatCtx(nullptr)
                    ,m_pInputFmt(nullptr)
                    ,m_pCodecCtx(nullptr)
                    ,m_pCodec(nullptr)
{
    Init();
}

CCamera::~CCamera()
{

}

void CCamera::Init()
{
    m_strInputFmt = "video4linux2";
    m_strInputFile = "/dev/video0";
    m_iVideoIndex = -1;

    Open();
}

void CCamera::Open()
{
    avdevice_register_all();
    m_pInputFmt = av_find_input_format(m_strInputFmt.c_str());
    if (m_pInputFmt == NULL)
    {
        std::cout<<"can not find input format. "<<std::endl;
        return;
    }

    if (avformat_open_input(&m_pFormatCtx, m_strInputFile.c_str(), m_pInputFmt, NULL) < 0)
    {
        std::cout << "can not open input file."<< std::endl;
        return;
    }

    for (int i = 0; i < m_pFormatCtx->nb_streams; i++)
    {
        if(m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            m_iVideoIndex = i;
            break;
        }
    }

    if(m_iVideoIndex == -1)
    {
        std::cout << "find video stream failed, now return." << std::endl;
        return;
    }

    m_pCodec = avcodec_find_decoder(m_pFormatCtx->streams[m_iVideoIndex]->codecpar->codec_id);
    if(m_pCodec == NULL)
    {
        std::cout << "Codec not found." << std::endl;
        return;
    }
    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
    avcodec_parameters_to_context(m_pCodecCtx, m_pFormatCtx->streams[m_iVideoIndex]->codecpar);
}

void CCamera::GetData(AVPacket* packet)
{
    av_read_frame(m_pFormatCtx, packet);
}

void CCamera::GetSize(int& width, int& height)
{
    width = m_pCodecCtx->width;
    height = m_pCodecCtx->height;
}

AVPixelFormat CCamera::GetPixFmt()
{
    return m_pCodecCtx->pix_fmt;
}
