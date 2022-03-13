#ifndef CCAMERASTREAMTEST_H
#define CCAMERASTREAMTEST_H

#include "ccamerastreamserver.h"
#include "ccamerastreamclient.h"

class CCameraStreamTest
{
public:
    CCameraStreamTest();
    void Start();

private:
    void Server();
    void Client();

private:
    CCameraStreamServer             m_objCameraStreamServer;
    CCameraStreamClient             m_objCameraStreamClient;
};

#endif // CCAMERASTREAMTEST_H
