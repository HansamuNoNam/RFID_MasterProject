QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += serialbus widgets
requires(qtConfig(combobox))
qtConfig(modbus-serialport): QT += serialport
CONFIG += c++11
QT += gui
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    configuredialog.cpp \
    main.cpp \
    mainwindow.cpp \
    masterthread.cpp \
    messageerrordialog.cpp \
    rfiddata.cpp

HEADERS += \
    configuredialog.h \
    mainwindow.h \
    masterthread.h \
    messageerrordialog.h \
    rfiddata.h\
    Endian.h

FORMS += \
    configuredialog.ui \
    mainwindow.ui \
    messageerrordialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
