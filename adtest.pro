#-------------------------------------------------
#
# Project created by QtCreator 2019-07-09T09:16:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT       += network
TARGET = adtest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    driver/driver_adcard16.cpp \
    driver/driver_t9342.cpp \
    driver/driver_t9342thread.cpp \
    driver/driver_virtualad16.cpp \
    driver/fifo.cpp \
    qcustomplot.cpp \
    scale/ch_adjust_mgr.cpp \
    scale/fltrcd.cpp \
    scale/rt_ctl.cpp \
    scale/rt_thd.cpp \
    scale/sg_mgr.cpp \
    scale/shd_ctl.cpp \
    scale/xtime.cpp \
    scale/calc_basic.c \
    scale/rcd_ctl.c \
    scale/rdu_len.c \
    scale/rt_sg.c \
    BasicAlgorithm.cpp \
    mySplashScreen.cpp

HEADERS  += mainwindow.h \
    driver/bdaqctrl.h \
    driver/driver_abstract_adcard.h \
    driver/driver_adcard16.h \
    driver/driver_dio.h \
    driver/driver_disk.h \
    driver/driver_t9342.h \
    driver/driver_t9342thread.h \
    driver/driver_virtualad16.h \
    driver/fifo.h \
    libuv/tree.h \
    libuv/uv.h \
    libuv/uv-errno.h \
    libuv/uv-threadpool.h \
    libuv/uv-version.h \
    libuv/uv-win.h \
    qcustomplot.h \
    scale/calc_basic.h \
    scale/calc_def.h \
    scale/ch_adjust_mgr.h \
    scale/fltrcd.h \
    scale/mkshd.h \
    scale/rcd_ctl.h \
    scale/rcd_msg.h \
    scale/rdu_len.h \
    scale/rt_ctl.h \
    scale/rt_sg.h \
    scale/rt_thd.h \
    scale/sg_def.h \
    scale/sg_mgr.h \
    scale/shc_ctl.h \
    scale/shd_ctl.h \
    scale/shd_def.h \
    scale/startpoint_find.h \
    scale/trg_def.h \
    scale/xtime.h \
    scale/yhftypes.h \
    BasicAlgorithm.h \
    yhftypes.h \
    MySplashScreen.h

FORMS    += mainwindow.ui
QT += widgets printsupport


unix|win32: LIBS += -L$$PWD/libuv/ -llibuv

INCLUDEPATH += $$PWD/libuv
DEPENDPATH += $$PWD/libuv

DISTFILES += \
    image/timg.jpg

RESOURCES += \
    image/src.qrc

