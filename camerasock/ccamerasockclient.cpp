#include "ccamerasockclient.h"
#include "clogutil.h"
#include <iostream>
#include "cdatapool.h"
#include <thread>

#define PIX_FMT

CCameraSockClient::CCameraSockClient() : CSDLPlayer()
  ,m_iSockConn(-1)
  ,m_arrRemoteAddr("127.0.0.1")
  ,m_iRemotePort(1234)
  ,m_pAVCodecCtx(nullptr)
  ,m_pAVcodec(nullptr)
{

}

void CCameraSockClient::Receive()
{
    int iLen = 0;
    uint8_t arrRcvBuf[eSendBufSize] = {0};
    std::cout << "before recv. " << std::endl;
    while ( (iLen = recv(m_iSockConn, arrRcvBuf, eSendBufSize, 0) ) > 0 )
    {
        std::cout << "recv" << std::endl;
        LogUtil.Log(CLogUtil::eDebug, "recv len:[%d]", iLen);
        CDataPool::GetInstance().AppendBufferData(arrRcvBuf, iLen);
    }
}

void CCameraSockClient::Start()
{
    if (OpenConnection())
    {
        std::thread t(&CCameraSockClient::Receive, this);
        t.detach();

        Play();
//        t.join();
    }
}

bool CCameraSockClient::OpenConnection()
{
    bool bRet = false;
    m_iSockConn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(m_arrRemoteAddr.c_str());
    serv_addr.sin_port = htons(m_iRemotePort);
    if ( 0 != connect(m_iSockConn, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) )
    {
        LogUtil.Log(CLogUtil::eWTF, "connect receiver failed. return");
        return bRet;
    }
    else
    {
        bRet = true;
    }
    return bRet;
}

void CCameraSockClient::InitCodec()
{
    m_pAVcodec = avcodec_find_decoder(AV_CODEC_ID_MPEG4);
    if (!m_pAVcodec)
    {
        std::cout << "find avcodec failed. " << std::endl;
        return;
    }
    m_pAVCodecCtx = avcodec_alloc_context3(m_pAVcodec);
    m_pAVCodecCtx->codec_id = m_pAVcodec->id;
    m_pAVCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    m_pAVCodecCtx->pix_fmt = (AVPixelFormat)ePixFmt;
    m_pAVCodecCtx->width = eVideoSizeW;
    m_pAVCodecCtx->height = eVideoSizeH;
    m_pAVCodecCtx->time_base.num =1;
    m_pAVCodecCtx->time_base.den = 2;
    m_pAVCodecCtx->bit_rate = 100000;
    m_pAVCodecCtx->gop_size=5;
    m_pAVCodecCtx->qmin = 10;
    m_pAVCodecCtx->qmax = 51;
    m_pAVCodecCtx->time_base = {1, 1};

    if(avcodec_open2(m_pAVCodecCtx, m_pAVcodec, nullptr) < 0)
    {
        std::cout << "avcodec open failed. " << std::endl;
        return;
    }
    m_pDecodeHelper = new CDecodeHelper(m_pAVCodecCtx);
}

void CCameraSockClient::InitFrameData()
{
    InitCodec();
    m_PixelW = eVideoSizeW;
    m_PixelH = eVideoSizeH;
    m_Pixformat = SDL_PIXELFORMAT_IYUV;

    m_FrameBuffSize = av_image_get_buffer_size((AVPixelFormat)ePixFmt, eVideoSizeW, eVideoSizeH, 16);
    m_pFrameBuff = (unsigned char *)av_malloc(m_FrameBuffSize);
    m_pFrame = av_frame_alloc();
}

void CCameraSockClient::GetRenderData(unsigned char*& pBuffer, int& iFrameDataPitch)
{
    pBuffer = nullptr;
    iFrameDataPitch = 0;

    AVPacket* pPacket = nullptr;
    if (CDataPool::GetInstance().TakeNextPacketData(pPacket))
    {
        if (m_pDecodeHelper->Decode(pPacket, m_pFrame) )
        {
            std::cout << "render. " << std::endl;
            av_image_copy_to_buffer(m_pFrameBuff, m_FrameBuffSize,
                    m_pFrame->data, m_pFrame->linesize,
                    (AVPixelFormat)ePixFmt,
                    eVideoSizeW, eVideoSizeH, 16);
            pBuffer = m_pFrameBuff;
            iFrameDataPitch = eVideoSizeW;
            av_packet_unref(pPacket);
            delete pPacket;
        }

        CDataPool::GetInstance().DisardNextPacketData();
    }
}
