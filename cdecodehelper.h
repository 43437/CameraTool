#ifndef CDECODEHELPER_H
#define CDECODEHELPER_H

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

class CDecodeHelper
{
public:
    CDecodeHelper(AVCodecContext *pAVCodecCtx);
    bool Decode(AVPacket* pAVPacket, AVFrame* pFrame);

private:
    AVCodecContext          *m_pAVCodecCtx;
};

#endif // CDECODEHELPER_H
