#ifndef CYUVSaver_H
#define CYUVSaver_H

#ifdef __cplusplus
extern "C"{
#endif
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libavdevice/avdevice.h>
  #include <libswscale/swscale.h>
  #include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif
#include "camera/ccamera.h"

class CYUVSaver
{
public:
    CYUVSaver(CCamera& camera);
    void Save();

private:
    void Init();

private:
    int                 m_iHeight;
    int                 m_iWidth;
    AVPixelFormat       m_ePixFmt;
    CCamera             &m_rCamera;
    string              m_strSaveFile;
};

#endif // CYUVSaver_H
