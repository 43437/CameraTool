#include "cdecodehelper.h"
#include <iostream>

CDecodeHelper::CDecodeHelper(AVCodecContext *pAVCodecCtx) : m_pAVCodecCtx(pAVCodecCtx)
{

}

bool CDecodeHelper::Decode(AVPacket* pAVPacket, AVFrame* pFrame)
{
    bool bRet = false;

    std::cout << "before decode. " << std::endl;
    if ( 0 != avcodec_send_packet(m_pAVCodecCtx, pAVPacket) )
    {
        std::cerr << "avcodec_send_packet failed. "<< std::endl;
        bRet = false;
    }

    //获取编码结果保存到输出流
    int iRet = 0;
    iRet = avcodec_receive_frame(m_pAVCodecCtx, pFrame);
    switch(iRet)
    {
    case 0:
        bRet = true;
        break;
    case AVERROR(EAGAIN):
        bRet = false;
        break;
    default:
        std::cout << "avcodec_receive_frame error."<< std::endl;
        bRet = false;
    break;
    }

    return bRet;
}
