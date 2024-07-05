QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ColonoscopyDataCollector

TEMPLATE = app

CONFIG += c++11

SOURCES += \
    ColonoscopeSensor.cpp \
    library/src/BufferedReader.cpp \
    library/src/ComConnection.cpp \
    library/src/CombinedApi.cpp \
    library/src/GbfButton1D.cpp \
    library/src/GbfComponent.cpp \
    library/src/GbfContainer.cpp \
    library/src/GbfData3D.cpp \
    library/src/GbfData6D.cpp \
    library/src/GbfFrame.cpp \
    library/src/GbfSystemAlert.cpp \
    library/src/MarkerData.cpp \
    library/src/PortHandleInfo.cpp \
    library/src/SystemAlert.cpp \
    library/src/SystemCRC.cpp \
    library/src/TcpConnection.cpp \
    library/src/ToolData.cpp \
    library/src/Transform.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    ColonoscopeSensor.h \
    WinTypes.h \
    ftd2xx.h \
    library/include/CombinedApi.h \
    library/include/MarkerData.h \
    library/include/PortHandleInfo.h \
    library/include/SystemAlert.h \
    library/include/ToolData.h \
    library/include/Transform.h \
    library/src/include/BufferedReader.h \
    library/src/include/ComConnection.h \
    library/src/include/Connection.h \
    library/src/include/GbfButton1D.h \
    library/src/include/GbfComponent.h \
    library/src/include/GbfContainer.h \
    library/src/include/GbfData3D.h \
    library/src/include/GbfData6D.h \
    library/src/include/GbfFrame.h \
    library/src/include/GbfSystemAlert.h \
    library/src/include/SystemCRC.h \
    library/src/include/TcpConnection.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui


INCLUDEPATH += /usr/include/opencv4

LIBS += `pkg-config --cflags --libs opencv4`
LIBS += -L$$quote($$PWD) -lftd2xx
