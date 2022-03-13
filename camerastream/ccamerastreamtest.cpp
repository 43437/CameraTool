#include "ccamerastreamtest.h"
#include <thread>
#include <unistd.h>

CCameraStreamTest::CCameraStreamTest()
{

}

void CCameraStreamTest::Server()
{
    m_objCameraStreamServer.Start();
}

void CCameraStreamTest::Client()
{
    sleep(3);
    m_objCameraStreamClient.Start();
}

void CCameraStreamTest::Start()
{
    std::thread thServer(&CCameraStreamTest::Server, this);
    std::thread thClient(&CCameraStreamTest::Client, this);

    thServer.join();
    thClient.join();
}
