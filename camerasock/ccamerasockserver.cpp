#include "ccamerasockserver.h"
#include "clogutil.h"
#include "cdatapool.h"
#include "cencodhelper.h"
#include "unistd.h"

CCameraSockServer::CCameraSockServer() :
  m_objCameraYUV(eVideoSizeW, eVideoSizeH, (AVPixelFormat)ePixFmt)
  ,m_arrLocalAddr("127.0.0.1")
  ,m_iLocalPort(1234)
  ,m_iSockConn(-1)
  ,m_pFrameBuff(nullptr)
  ,m_pFrame(nullptr)
  ,m_pAVCodecCtx(nullptr)
  ,m_pAVcodec(nullptr)
{
    Init();
}

void CCameraSockServer::InitCodec()
{
    m_pAVcodec = avcodec_find_encoder(AV_CODEC_ID_MPEG4);
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
}

void CCameraSockServer::InitDataBuf()
{
    int iDataSize = m_objCameraYUV.GetDataSize();
    m_pFrame = av_frame_alloc();
    m_pFrame->width = eVideoSizeW;
    m_pFrame->height = eVideoSizeH;
    m_pFrame->format = (AVPixelFormat)ePixFmt;
    m_pFrameBuff = (uint8_t *)av_malloc(iDataSize);
    av_image_fill_arrays(m_pFrame->data, m_pFrame->linesize, m_pFrameBuff, (AVPixelFormat)ePixFmt, eVideoSizeW, eVideoSizeH, 16);
}

void CCameraSockServer::Init()
{
    InitDataBuf();

    InitCodec();
}

void CCameraSockServer::Start()
{
    Server();
}

void CCameraSockServer::Server()
{
    //网络
    int iServSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //将套接字和IP、端口绑定
    struct sockaddr_in stuServAddr;
    memset(&stuServAddr, 0, sizeof(stuServAddr));  //每个字节都用0填充
    stuServAddr.sin_family = AF_INET;  //使用IPv4地址
    stuServAddr.sin_addr.s_addr = inet_addr(m_arrLocalAddr.c_str());  //具体的IP地址
    stuServAddr.sin_port = htons(m_iLocalPort);  //端口
    bind(iServSock, (struct sockaddr*)&stuServAddr, sizeof(stuServAddr));

    //进入监听状态，等待用户发起请求
    listen(iServSock, 20);

    //接收客户端请求
    struct sockaddr_in stuClntAddr;
    socklen_t lenAddrSize = sizeof(stuClntAddr);
    m_iSockConn = accept(iServSock, (struct sockaddr*)&stuClntAddr, &lenAddrSize);

    CEncodeHelper outputEncoder(m_pAVCodecCtx);
    AVPacket stuOutPkt;
    memset(&stuOutPkt, 0, sizeof(AVPacket));
    std::cout << "before transport." << std::endl;
    while (true)
    {
        m_objCameraYUV.GetData(m_pFrameBuff);
        outputEncoder.Encode(m_pFrame, &stuOutPkt);
        Publish(stuOutPkt);
        av_packet_unref(&stuOutPkt);
        //500ms
        usleep(500 * 1000);
    }
}

void CCameraSockServer::Publish(AVPacket& stuOutPkt)
{
    const int iLen = DATA_SPLIT_LEN + sizeof(stuOutPkt.pts) + sizeof(stuOutPkt.dts) + sizeof(stuOutPkt.stream_index) + sizeof(stuOutPkt.flags) + sizeof(stuOutPkt.size) + stuOutPkt.size + DATA_SPLIT_LEN;
    int iIndex = 0;
    memcpy(&m_arrSendBuf[iIndex], BEGIN_FLAG, DATA_SPLIT_LEN);
    iIndex += DATA_SPLIT_LEN;
    memcpy(&m_arrSendBuf[iIndex], &stuOutPkt.pts, sizeof(stuOutPkt.pts));	//pts
    iIndex += sizeof(stuOutPkt.pts);
    memcpy(&m_arrSendBuf[iIndex], &stuOutPkt.dts, sizeof(stuOutPkt.dts));	//dts
    iIndex += sizeof(stuOutPkt.dts);
    memcpy(&m_arrSendBuf[iIndex], &stuOutPkt.stream_index, sizeof(stuOutPkt.stream_index));	//stream_index
    iIndex += sizeof(stuOutPkt.stream_index);
    memcpy(&m_arrSendBuf[iIndex], &stuOutPkt.flags, sizeof(stuOutPkt.flags));	//flags
    iIndex += sizeof(stuOutPkt.flags);
    memcpy(&m_arrSendBuf[iIndex], &stuOutPkt.size, sizeof(stuOutPkt.size));	//size
    iIndex += sizeof(stuOutPkt.size);

    memcpy(&m_arrSendBuf[iIndex], stuOutPkt.data, stuOutPkt.size);	//data
    iIndex += stuOutPkt.size;
    memcpy(&m_arrSendBuf[iIndex], END_FLAG, DATA_SPLIT_LEN);

    int iSendLen = 0;
    if ( ( iSendLen = send(m_iSockConn, m_arrSendBuf, iLen, 0) ) < 0)
    {
        std::cout << "send faield" << std::endl;
        LogUtil.Log(CLogUtil::eWTF, "CVideoCamera::publish failed. ");
    }
    else{
        std::cout << "send" << std::endl;
        LogUtil.Log(CLogUtil::eDebug, "data iLen: [%d] send len: [%d]", iLen, iSendLen);
    }
}
