#include "cencodhelper.h"
#include <iostream>

CEncodeHelper::CEncodeHelper(AVCodecContext *pAVCodecCtx) : m_pAVCodecCtx(pAVCodecCtx)
{
    Init();
}

bool CEncodeHelper::Encode(AVFrame* pFrame, AVPacket* pAVPacket)
{
    bool bRet = false;

    if ( 0 != avcodec_send_frame(m_pAVCodecCtx, pFrame) )
    {
        std::cerr<<"avcodec_send_frame failed. "<<std::endl;
        bRet = false;
    }

    //获取编码结果保存到输出流
    int iRet = 0;
    iRet = avcodec_receive_packet(m_pAVCodecCtx, pAVPacket);
    switch(iRet)
    {
    case 0:
        bRet = true;
        break;
    case AVERROR(EAGAIN):
        bRet = false;
        break;
    default:
        std::cout << "avcodec_receive_packet error."<< std::endl;
        bRet = false;
    break;
    }

    return bRet;
}

void CEncodeHelper::Init()
{

}
