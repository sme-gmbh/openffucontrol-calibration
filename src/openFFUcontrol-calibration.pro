QT       += core gui
QT += serialport
QT += testlib
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    agilent34970a.cpp \
    main.cpp \
    mainwindow.cpp \
    openffucontrolpts.cpp \
    pdfoutput.cpp

LIBS     += -lmodbus

HEADERS += \
    agilent34970a.h \
    mainwindow.h \
    openffucontrolpts.h \
    pdfoutput.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
