#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T11:21:58
#
#-------------------------------------------------

QT      += core gui
QT      += network
QT      += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cassapa_viewer
TEMPLATE = app

INCLUDEPATH += ../../../code/detector \
    ../../../code/viewer \
    C:/projetos/libs/opencv/OpenCV-2.4.7.1-android-sdk/sdk/native/jni/include


SOURCES +=\
        mainwindow.cpp \
    ../../../code/viewer/config_viewer.cpp \
    ../../../code/viewer/configborders.cpp \
    ../../../code/viewer/configurecolors.cpp \
    ../../../code/viewer/ctkDoubleRangeSlider.cpp \
    ../../../code/viewer/ctkRangeSlider.cpp \
    ../../../code/viewer/image_receiver.cpp \
    ../../../code/viewer/image_receiver2.cpp \
    ../../../code/viewer/kassapa_viewer.cpp \
    ../../../code/viewer/menu.cpp \
    ../../../code/viewer/preview.cpp \
    ../../../code/viewer/socket_viewer.cpp \
    ../../../code/viewer/main.cpp

HEADERS  += mainwindow.h \
    ../../../code/viewer/config_viewer.h \
    ../../../code/viewer/configborders.h \
    ../../../code/viewer/configurecolors.h \
    ../../../code/viewer/ctkDoubleRangeSlider.h \
    ../../../code/viewer/ctkRangeSlider.h \
    ../../../code/viewer/ctkWidgetsExport.h \
    ../../../code/viewer/image_receiver.h \
    ../../../code/viewer/image_receiver2.h \
    ../../../code/viewer/kassapa_viewer.h \
    ../../../code/viewer/menu.h \
    ../../../code/viewer/preview.h \
    ../../../code/viewer/socket_viewer.h \
    main.cpp.old

FORMS    += mainwindow.ui \
    ../../../code/viewer/configborders.ui \
    ../../../code/viewer/configurecolors.ui \
    ../../../code/viewer/kassapa_viewer.ui \
    ../../../code/viewer/menu.ui \
    ../../../code/viewer/preview.ui

CONFIG += mobility
MOBILITY = 

OTHER_FILES += \
    ../../../code/viewer/kassapa_viewer.aps

RESOURCES += \
    ../../../code/viewer/kassapa_viewer.qrc

