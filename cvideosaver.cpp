#include "cvideosaver.h"
#include <iostream>
#include "coutputencoder.h"
#include <unistd.h>

CVideoSaver::CVideoSaver(CCamera& camera) : m_rCamera(camera)
  ,m_pFormatCtx(nullptr)
  ,m_pAVCodecCtx(nullptr)
  ,m_pAVcodec(nullptr)
  ,m_strSaveFile("out.mp4")
  ,m_iVideoIndex(-1)
  ,m_iSaveWidth(1280)
  ,m_iSaveHeight(720)
  ,m_eDstPixFmt(AV_PIX_FMT_YUV420P)
{
    Init();
}

void CVideoSaver::Init()
{
    const AVOutputFormat *pOutFmt = av_guess_format("mp4", nullptr, nullptr);
    if (nullptr == pOutFmt)
    {
        std::cout << "av guess format failed, now return. " << std::endl;
        return;
    }
    m_pFormatCtx = avformat_alloc_context();
    if (avio_open(&m_pFormatCtx->pb, m_strSaveFile.c_str(), AVIO_FLAG_READ_WRITE) < 0)
    {
        std::cout << "Failed to open output file! \n" << std::endl;
        return;
    }
    m_pFormatCtx->oformat = pOutFmt;

    AVStream* pOStream = avformat_new_stream(m_pFormatCtx, NULL);
    pOStream->codecpar->codec_id    = pOutFmt->video_codec;
    pOStream->codecpar->codec_type  = AVMEDIA_TYPE_VIDEO;
    pOStream->codecpar->width       = m_iSaveWidth;
    pOStream->codecpar->height      = m_iSaveHeight;
    pOStream->codecpar->format      = AV_PIX_FMT_YUV420P;
//    pOStream->codecpar->codec_tag   = *(pOutFmt->codec_tag)->tag;

    if (!pOStream)
    {
        std::cout << "new stream failed, now return. " << std::endl;
        return;
    }

    m_iVideoIndex = pOStream->index;
    if (-1 == m_iVideoIndex)
    {
        std::cout << "CJPGSaver::Init find m_iVideoIndex fail" << std::endl;
    }
    m_pAVcodec = avcodec_find_encoder(pOutFmt->video_codec);
    if (!m_pAVcodec)
    {
        std::cout << "find avcodec failed. " << std::endl;
        return;
    }
    m_pAVCodecCtx = avcodec_alloc_context3(m_pAVcodec);

    avcodec_parameters_to_context(m_pAVCodecCtx, m_pFormatCtx->streams[m_iVideoIndex]->codecpar);
    m_pAVCodecCtx->time_base = {1, 1};
    m_pAVCodecCtx->color_range = AVCOL_RANGE_JPEG;
    m_pAVCodecCtx->codec_id = pOutFmt->video_codec;
    m_pAVCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    m_pAVCodecCtx->pix_fmt = m_eDstPixFmt;
    m_pAVCodecCtx->width = m_iSaveWidth;
    m_pAVCodecCtx->height = m_iSaveHeight;
    m_pAVCodecCtx->time_base.num =1;
    m_pAVCodecCtx->time_base.den = 25;
    m_pAVCodecCtx->bit_rate = 400000;
    m_pAVCodecCtx->gop_size=250;
    m_pAVCodecCtx->qmin = 10;
    m_pAVCodecCtx->qmax = 51;
    if(avcodec_open2(m_pAVCodecCtx, m_pAVcodec, nullptr) < 0)
    {
        std::cout<<"avcodec open failed. "<<std::endl;
        return;
    }
}

void CVideoSaver::Save()
{
    uint8_t     *srcData[4];
    uint8_t     *dstData[4];
    int         srcStride[4];
    int         dstStride[4];
    int         iSrcWidth = 1280;
    int         iSrcHeight = 720;

    AVPixelFormat eDstPixFmt = AV_PIX_FMT_YUV420P;

    //Get YUV data size
    m_rCamera.GetSize(iSrcWidth, iSrcHeight);

    //Save jpg begin
    int iRet = avformat_write_header(m_pFormatCtx, NULL);
    switch(iRet)
    {
    case AVSTREAM_INIT_IN_WRITE_HEADER:
      std::cout<<"avformat_write_header AVSTREAM_INIT_IN_WRITE_HEADER"<<std::endl;
      break;
    case AVSTREAM_INIT_IN_INIT_OUTPUT:
      std::cout<<"avformat_write_header AVSTREAM_INIT_IN_INIT_OUTPUT"<<std::endl;
      break;
    default:
      break;
    }

    //Scale begin
    av_image_alloc(srcData, srcStride, iSrcWidth, iSrcHeight, m_rCamera.GetPixFmt(), 16);
    int dstSize = av_image_alloc(dstData, dstStride, iSrcWidth, iSrcHeight, eDstPixFmt, 16);

    AVFrame *pFrame = av_frame_alloc();
    uint8_t *pFrameBuff = (uint8_t *)av_malloc(dstSize);

    //let pFrame's member pointer point to pFrameBuff pointer
    av_image_fill_arrays(pFrame->data, pFrame->linesize, pFrameBuff, eDstPixFmt, m_iSaveWidth, m_iSaveHeight, 16);
    pFrame->width = m_iSaveWidth;
    pFrame->height = m_iSaveHeight;
    pFrame->format= eDstPixFmt;

    //scale the input frame to ouput frame
    SwsContext* swsCtx = sws_getContext(iSrcWidth, iSrcHeight, m_rCamera.GetPixFmt(), m_iSaveWidth, m_iSaveHeight, eDstPixFmt, SWS_BILINEAR, NULL, NULL, NULL);

    AVPacket stuInPkt;
    AVPacket stuOutPkt;
    memset(&stuInPkt, 0, sizeof(AVPacket));
    memset(&stuOutPkt, 0, sizeof(AVPacket));
    int i = 0;
    while (true)
    {
        if (i > 10)
            break;
        ++i;
        m_rCamera.GetData(&stuInPkt);
        memcpy(srcData[0], stuInPkt.data, stuInPkt.size);
        sws_scale(swsCtx, srcData, srcStride, 0, iSrcHeight, dstData, dstStride);

        //when copy data to pFrameBuff, that means the data fill in pFrame
        memcpy(pFrameBuff, dstData[0], dstSize);
        //Scale end

        //Encode begin
//        COutputEncoder outEncoder(m_pAVCodecCtx);

//        AVPacket *pSavePacket = av_packet_alloc();
        pFrame->pts = i * (m_pAVCodecCtx->time_base.den) / ((m_pAVCodecCtx->time_base.num) * 25);
        if ( 0 != avcodec_send_frame(m_pAVCodecCtx, pFrame) )
        {
            std::cerr<<"avcodec_send_frame failed. "<<std::endl;
//            bRet = false;
        }

        //获取编码结果保存到输出流
        bool bContinue = true;
        while (bContinue)
        {
            int iRet = 0;
            iRet = avcodec_receive_packet(m_pAVCodecCtx, &stuOutPkt);
            switch(iRet)
            {
            case 0:
                bContinue = true;
                stuOutPkt.stream_index = m_iVideoIndex;
                av_write_frame(m_pFormatCtx, &stuOutPkt);
                av_packet_unref(&stuOutPkt);
                break;
            case AVERROR(EAGAIN):
                bContinue = false;
                break;
            default:
                std::cout << "avcodec_receive_packet error."<< std::endl;
                bContinue = false;
            break;
            }
        }
//        outEncoder.Encode(pFrame, &stuOutPkt);
        //Encode end

        sleep(1);
    }

    av_write_frame(m_pFormatCtx, NULL);
    av_write_trailer(m_pFormatCtx);
    //save jpg end
}
