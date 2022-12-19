QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminwindow.cpp \
    bookdialog.cpp \
    flightwindow.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    adminwindow.h \
    bookdialog.h \
    database.h \
    flightwindow.h \
    MainWindow.h

FORMS += \
    adminwindow.ui \
    bookdialog.ui \
    flightwindow.ui \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
