QT       += core gui printsupport sql axcontainer widgets xml svg network positioning serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    animatedlabel.cpp \
    calc.cpp \
    datawindow.cpp \
    equipwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    maplayer.cpp \
    qcustomplot.cpp \
    resultwindow.cpp \
    settingswindow.cpp

HEADERS += \
    airbase.h \
    animatedlabel.h \
    calc.h \
    datawindow.h \
    equipwindow.h \
    includes.h \
    mainwindow.h \
    maplayer.h \
    plane.h \
    qcustomplot.h \
    random.h \
    resultwindow.h \
    settingswindow.h

FORMS += \
    datawindow.ui \
    equipwindow.ui \
    mainwindow.ui \
    resultwindow.ui \
    settingswindow.ui

win32:RC_FILE = mainico.rc

INCLUDEPATH += "$$quote(marble)"

DEPENDPATH +=  $$quote(marble)

LIBS += $$quote(./libmarblewidget-qt5.dll) $$quote(./libastro.dll) $$quote(./libmarbledeclarative.dll)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
