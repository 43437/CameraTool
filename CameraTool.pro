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
        ccamera.cpp \
        ccameraplayer.cpp \
        cjpgsaver.cpp \
        coutputencoder.cpp \
        csdlplayer.cpp \
        cvideosaver.cpp \
        cyuvsaver.cpp \
        main.cpp

HEADERS += \
    ccamera.h \
    ccameraplayer.h \
    cjpgsaver.h \
    coutputencoder.h \
    csdlplayer.h \
    cvideosaver.h \
    cyuvsaver.h

DISTFILES += \
    README.md
