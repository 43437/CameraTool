#ifndef CCAMERASOCKTEST_H
#define CCAMERASOCKTEST_H

#include "ccamerasockserver.h"
#include "ccamerasockclient.h"
#include "ccamerasockclient.h"
#include "ccamerasockserver.h"

class CCameraSockTest
{
public:
    CCameraSockTest();
    void Start();

private:
    void Server();
    void Client();

private:
    CCameraSockServer               m_objServer;
    CCameraSockClient               m_objClient;
};

#endif // CCAMERASOCKTEST_H
