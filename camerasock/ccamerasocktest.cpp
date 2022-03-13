#include "ccamerasocktest.h"
#include <unistd.h>
#include <thread>

CCameraSockTest::CCameraSockTest()
{

}

void CCameraSockTest::Server()
{
    m_objServer.Start();
}

void CCameraSockTest::Client()
{
    sleep(3);
    m_objClient.Start();
}

void CCameraSockTest::Start()
{
    std::thread thServer(&CCameraSockTest::Server, this);
    std::thread thClient(&CCameraSockTest::Client, this);

    thServer.join();
    thClient.join();
}
