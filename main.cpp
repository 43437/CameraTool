#include <iostream>
#include "camera/ccamera.h"
#include "saver/cyuvsaver.h"
#include "saver/cjpgsaver.h"
#include "saver/cvideosaver.h"
#include "sdlplayer/ccameraplayer.h"
#include "camerasock/ccamerasocktest.h"
#include "camerastream/ccamerastreamtest.h"

using namespace std;

int main()
{
//    CCamera camera;
//    CYUVSaver yuvSaver(camera);
//    yuvSaver.Save();

//    CJPGSaver jpgSaver(camera);
//    jpgSaver.Save();

//    CVideoSaver videoSaver(camera);
//    videoSaver.Save();

//    CCameraPlayer cameraPlayer(camera);
//    cameraPlayer.Play();

    CCameraSockTest cameraSockTest;
    cameraSockTest.Start();

    cout << "Hello World!" << endl;
    return 0;
}
