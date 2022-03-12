#include <iostream>
#include "ccamera.h"
#include "cyuvsaver.h"
#include "cjpgsaver.h"
#include "cvideosaver.h"
#include "ccameraplayer.h"

using namespace std;

int main()
{
    CCamera camera;
//    CYUVSaver yuvSaver(camera);
//    yuvSaver.Save();

//    CJPGSaver jpgSaver(camera);
//    jpgSaver.Save();

//    CVideoSaver videoSaver(camera);
//    videoSaver.Save();

    CCameraPlayer cameraPlayer(camera);
    cameraPlayer.Play();

    cout << "Hello World!" << endl;
    return 0;
}
