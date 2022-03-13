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

SOURCES += \
        camerasock/ccamerasockclient.cpp \
        camerasock/ccamerasockserver.cpp \
        camerasock/ccamerasocktest.cpp \
        camerasock/cdatapool.cpp \
        ccamera.cpp \
        clogutil.cpp \
        coutputencoder.cpp \        
        main.cpp \
        saver/cvideosaver.cpp \
        saver/cyuvsaver.cpp \
        saver/cjpgsaver.cpp \
        sdlplayer/ccameraplayer.cpp \
        sdlplayer/csdlplayer.cpp

HEADERS += \
    camerasock/ccamerasockclient.h \
    camerasock/ccamerasockserver.h \
    camerasock/ccamerasocktest.h \
    camerasock/cdatapool.h \
    ccamera.h \
    clogutil.h \
    coutputencoder.h \
    saver/cvideosaver.h \
    saver/cyuvsaver.h \
    saver/cjpgsaver.h \
    sdlplayer/ccameraplayer.h \
    sdlplayer/csdlplayer.h

DISTFILES += \
    README.md
