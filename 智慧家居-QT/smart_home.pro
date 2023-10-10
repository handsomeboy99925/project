QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = app

SOURCES += \
    device/device_ac.cpp \
    device/device_curtain.cpp \
    device/device_fan.cpp \
    device/device_lamp.cpp \
    main.cpp \
    home.cpp \
    my_custom/digital_scroller.cpp \
    my_custom/global.cpp \
    my_custom/sliderbutton.cpp \
    tcp/tcpserver.cpp \
    tcp/tcpserver_thread.cpp \
    tcp/tcpsocket.cpp

HEADERS += \
    device/device_ac.h \
    device/device_curtain.h \
    device/device_fan.h \
    device/device_lamp.h \
    home.h \
    my_custom/digital_scroller.h \
    my_custom/global.h \
    my_custom/sliderbutton.h \
    tcp/tcpserver.h \
    tcp/tcpserver_thread.h \
    tcp/tcpsocket.h

FORMS += \
    device/device_ac.ui \
    device/device_curtain.ui \
    device/device_fan.ui \
    device/device_lamp.ui \
    home.ui \
    my_custom/digital_scroller.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc
