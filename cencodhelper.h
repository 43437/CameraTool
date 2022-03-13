#ifndef CEncodeHelper_H
#define CEncodeHelper_H

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

class CEncodeHelper
{
public:
    CEncodeHelper(AVCodecContext *pAVCodecCtx);
    bool Encode(AVFrame* pFrame, AVPacket* pAVPacket);

private:
    void Init();

private:
    AVCodecContext          *m_pAVCodecCtx;
};

#endif // CEncodeHelper_H
