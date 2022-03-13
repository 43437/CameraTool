TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /home/joseph/tools/ffmpeg_build/include/
LIBS += -L/home/joseph/tools/ffmpeg_build/lib/ -lavcodec \
        -lavdevice \
        -lavfilter \
        -lavformat \
        -lavutil \
        -lswresample \
        -lswscale

LIBS += -lSDL2
LIBS += -lpthread

SOURCES += \
        camera/ccamerayuv.cpp \
        camerasock/ccamerasockclient.cpp \
        camerasock/ccamerasockserver.cpp \
        camerasock/ccamerasocktest.cpp \
        camerasock/cdatapool.cpp \
        camera/ccamera.cpp \
        camerastream/ccamerastreamclient.cpp \
        camerastream/ccamerastreamserver.cpp \
        camerastream/ccamerastreamtest.cpp \
        cdecodehelper.cpp \
        cencodhelper.cpp \
        clogutil.cpp \
        main.cpp \
        saver/cvideosaver.cpp \
        saver/cyuvsaver.cpp \
        saver/cjpgsaver.cpp \
        sdlplayer/ccameraplayer.cpp \
        sdlplayer/csdlplayer.cpp

HEADERS += \
    camera/ccamerayuv.h \
    camerasock/ccamerasockclient.h \
    camerasock/ccamerasockserver.h \
    camerasock/ccamerasocktest.h \
    camerasock/cdatapool.h \
    camera/ccamera.h \
    camerastream/ccamerastreamclient.h \
    camerastream/ccamerastreamserver.h \
    camerastream/ccamerastreamtest.h \
    cdecodehelper.h \
    cencodhelper.h \
    clogutil.h \
    saver/cvideosaver.h \
    saver/cyuvsaver.h \
    saver/cjpgsaver.h \
    sdlplayer/ccameraplayer.h \
    sdlplayer/csdlplayer.h

DISTFILES += \
    README.md
