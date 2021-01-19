QT       += core gui
QT += charts
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += C:\Users\Administrator\eclipse-workspace\fetch2
INCLUDEPATH += C:\msys64\home\Administrator\workspace_msys2\rapidjson\include
LIBS += -LC:\Users\Administrator\eclipse-workspace\fetch2\Debug -lfetch2

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
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
    chart.cpp \
    chartview.cpp \
    http.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    chart.h \
    chartview.h \
    http.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
