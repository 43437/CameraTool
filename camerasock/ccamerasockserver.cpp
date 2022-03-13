#include "ccamerasockserver.h"
#include "clogutil.h"
#include "cdatapool.h"

CCameraSockServer::CCameraSockServer(CCamera& rCamera) : m_rCamera(rCamera)
  ,m_arrLocalAddr("127.0.0.1")
  ,m_iLocalPort(1234)
  ,m_iSockConn(-1)
{
    Init();
}

void CCameraSockServer::Init()
{

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

    AVPacket stuOutPkt;
    while (true)
    {
//        m_rCamera.GetData();
        Publish(stuOutPkt);
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
        LogUtil.Log(CLogUtil::eWTF, "CVideoCamera::publish failed. ");
    }
    else{
        LogUtil.Log(CLogUtil::eDebug, "data iLen: [%d] send len: [%d]", iLen, iSendLen);
    }
}
